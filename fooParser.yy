%require "3.2"
%language "c++"

%code requires {
    #include <string>
    #include <vector>
    #include <map>
    #include <sstream>
    // #include <stack>
    #include "location_t.hh"
    #include "FooLexer.hh"

    typedef foo::loc_type YYLTYPE;
    # define YYLTYPE_IS_DECLARED 1
}

%define api.namespace {foo}
%define api.parser.class {FooBisonParser}
%define api.value.type {foo::bison_value_type}
%define api.location.type {location_t}

%locations
%define parse.error detailed
%define parse.trace

%header
%verbose

%parse-param {FooLexer &lexer}
%parse-param {const bool debug}
%parse-param {std::ostream &out}

%initial-action
{
    #if YYDEBUG != 0
        set_debug_level(debug);
    #endif
};

%code {
    namespace foo
    {
        using std::map, std::vector, std::string, std::ostream, std::pair;
        using Parser=FooBisonParser;
        template<typename RHS>
        void calcLocation(location_t &current, const RHS &rhs, const std::size_t n);

        #define get_addr() gaddr(parser,location)
        #define push(out)    ppush(parser,location,out)
        #define pop(out)    ppop(parser,location,out)
        #define push_env(out)    ppush_env(parser,location,out)
        #define pop_env(out)    ppop_env(parser,location,out)

        map<string, addr_type>    var_addr;
        string env;
        vector<string> loop_continue,loop_break;

        class value_type{
        public:
            virtual bool ppush(Parser *parser,location_t location,ostream &out)=0;
            virtual bool is_const()=0;
            virtual bool in_stack(){
                return false;
            }
            virtual addr_type gaddr(Parser*,location_t)=0;
            virtual const_type get_const()=0;
        };
        class const_value_type: public value_type{
        public:
            const_type val;
            const_value_type(const_type val):val(val){}
            bool ppush(Parser *parser,location_t location,ostream &out);
            bool is_const(){
                return true;
            }
            virtual addr_type gaddr(Parser *parser, location_t location){
                return 0;
            }
            const_type get_const(){
                return val;
            }
        };
        class var_type: public value_type{
        public:
            // int_type intx;
            var_type(){

            }
            bool ppush(Parser *parser,location_t location,ostream &out){
                out<<"PUSH "<<get_addr()<<"\n";
                return true;
            }
            bool ppop(Parser *parser,location_t location,ostream &out){
                out<<"POP "<<get_addr()<<"\n";
                return true;
            }
            bool is_const(){
                return false;
            }
            // bool is_void(){
            //     return get_addr()==-1;
            // }
            virtual addr_type gaddr(Parser *parser,location_t location)=0;
            const_type get_const(){
                return 0;
            }
        };

        class name_var:public var_type{
        public:
            string name;
            name_var(string name):name(name){}
            addr_type gaddr(Parser *parser,location_t location){
                if(!var_addr.count(name)){
                    parser->error(location,"Undefined variable: "+name);
                    return -1;
                }
                return var_addr[name];
            }
            // bool in_stack(){
            //     return name==stack_var_name;
            // }
        };

        class addr_var:public var_type{
        public:
            addr_type addr;
            addr_var(addr_type addr):addr(addr){}
            addr_type gaddr(Parser *parser,location_t location){
                return addr;
            }
        };

        addr_var var_void(-1);
        const_value_type const_zero(0);
        // const string stack_var_name = "__stack_var_name";
        name_var A("__A"),B("__B");

        bool const_value_type::ppush(Parser *parser,location_t location,ostream &out){
            out<<"MOV "<<A.get_addr()<<" "<<val<<"\n";
            out<<"PUSH "<<A.get_addr()<<"\n";
            return true;
        }
        struct FnInfo{
            bool defined;
            // int arg_cnt;
            int_type ret_type;
            vector<int_type> types;
            vector<name_var> syms;  //dummy args
            vector<pair<string,addr_type>> defs;
            vector<addr_type> addrs;
        };

        map<string, FnInfo> fn_table;

        void ppush_env(Parser *parser,location_t location,ostream &out){
            auto &fn_env=fn_table[env];
            for(int i=0;i<(int)fn_env.syms.size();i++){
                fn_env.syms[i].push(out);
            }
            for(int i=0;i<(int)fn_env.defs.size();i++){
                addr_var(fn_env.defs[i].second).push(out);
            }
        }

        void ppop_env(Parser *parser,location_t location,ostream &out){
            auto &fn_env=fn_table[env];
            for(int i=(int)fn_env.syms.size()-1;i>=0;i--){
                fn_env.syms[i].pop(out);
            }
            for(int i=(int)fn_env.defs.size()-1;i>=0;i--){
                addr_var(fn_env.defs[i].second).pop(out);
            }
        }

        class Ast{
        public:
            Parser *parser;
            location_t location;
            Ast(Parser* parser, location_t location): parser(parser), location(location){}
        };

        void FnDec(Parser *parser, location_t location, int_type ret_type, string name, 
        vector<int_type> types,vector<addr_type> addrs)
        {
            if(fn_table.count(name)){
                auto &fn=fn_table[name];
                if(fn.ret_type != ret_type || fn.types != types || fn.addrs != addrs){
                    parser->error(location, "Ambiguous declarion of function "+name+".");
                }
            }
            auto &fn=fn_table[name];
            fn.ret_type=ret_type;
            fn.types=types;
            fn.addrs=addrs;
        }


        struct opt_type{
            string s;
            opt_type(string s):s(s){

            }
            bool operator==(string ss){return s==ss;}
        };


        struct Expr: public Ast{
            Expr(Parser *parser, location_t location):Ast(parser,location){

            }
            virtual value_type* eval(ostream &out)=0;
            virtual bool is_var(){
                return false;
            }
            virtual bool ends_with_ret()const{
                return false;
            }
        };
        struct Typelist:public Expr{
            vector<int_type> types;
            Typelist(Parser *parser,location_t location,vector<int_type> types)
            :Expr(parser,location),types(types){}
            value_type *eval(ostream &out){
                return &var_void;
            }
        };
        struct Deflist:public Expr{
            vector<int_type> types;
            vector<name_var> syms;
            vector<addr_type> addrs;
            Deflist(Parser *parser,location_t location,
            vector<int_type> types,vector<name_var> syms,vector<addr_type> addrs)
            :Expr(parser,location),
            types(types),syms(syms),addrs(addrs){}
            value_type *eval(ostream &out){
                return &var_void;
            }
        };
        class ValueExpr: public Expr{
        public:
            value_type *val;
            ValueExpr(Parser *parser, location_t location, value_type *val):Expr(parser,location),val(val){}
            value_type* eval(ostream &out){
                return val;
            }
            bool is_var(){
                return true;
            }
        };

        value_type* calc_bin(Parser *parser, location_t location, string o,value_type* xx,value_type* yy){
            using cn=const_value_type;
            if(!xx->is_const() || !yy->is_const()) return &var_void;
            int x=xx->get_const(),y=yy->get_const();
            if(o=="+")return new cn(x+y);
            if(o=="-")return new cn(x-y);
            if(o=="*")return new cn(x*y);
            if(o=="/"){
                if(y==0){
                    parser->error(location,"division by 0, seen as 0");
                    return new cn(0);
                }
                return new cn(x/y);
            }
            if(o=="%"){
                if(y==0){
                    parser->error(location,"mod by 0, seen as 0");
                    return new cn(0);
                }
                return new cn(x%y);
            }
            if(o=="|")return new cn(x|y);
            if(o=="^")return new cn(x^y);
            if(o=="&")return new cn(x&y);
            if(o=="<<"){
                if(y<0){
                    parser->error(location,"shift by a negative number, seen as 0");
                    return new cn(0);
                }
                return new cn(x<<y);
            }
            if(o==">>"){
                if(y<0){
                    parser->error(location,"shift by a negative number, seen as 0");
                    return new cn(0);
                }
                return new cn(x>>y);
            }
            if(o=="<")return new cn(x<y);
            if(o=="<=")return new cn(x<=y);
            if(o==">")return new cn(x>y);
            if(o==">=")return new cn(x>=y);
            if(o=="==")return new cn(x==y);
            if(o=="!=")return new cn(x!=y);
            return &var_void;
        }

        class Opt: public Expr{
        public:
            string id;
            opt_type opt;
            vector<Expr*> args;
            Opt(Parser* parser, location_t location, opt_type opt, vector<Expr*> args)
            :Expr(parser,location),id(to_str(location.first_line,location.first_column)),opt(opt),args(args){
                // std::cerr<<opt.s<<' '<<args.size()<<'\n';
            }
            value_type* eval(ostream &out){
                // std::cerr<<id<<' '<<opt.s<<' '<<args.size()<<"\n";
                // for(int i=0;i<=(int)args.size()-1;i++) std::cerr<<args[i]<<' ';
                // std::cerr<<std::endl;
                // if(opt=="<<"||opt=="|"){
                //     0;
                // }
                if(opt=="?:"){
                    auto itv=args[0]->eval(out);
                    if(itv->is_const()){
                        if(itv->get_const()){
                            return args[1]->eval(out);
                        }
                        else{
                            return args[2]->eval(out);
                        }
                    }
                    else{
                        out << "JZ " << itv->get_addr() << " __TERNARY_ELSE_"<<id<<"\n";
                    }
                    auto it=new Opt(parser,location,opt_type("="),
                        {new ValueExpr(parser,location,&A),
                         new ValueExpr(parser,location,args[1]->eval(out))});
                    it->eval(out);
                    out << "JMP __TERNARY_END_"<<id<<"\n";
                    out << "label __TERNARY_ELSE_"<<id<<"\n";
                    it=new Opt(parser,location,opt_type("="),
                        {new ValueExpr(parser,location,&A),
                         new ValueExpr(parser,location,args[2]->eval(out))});
                    it->eval(out);
                    out << "label __TERNARY_END_"<<id<<"\n";
                    return &A;
                }
                if(args.size()==2){
                    if(opt=="="){
                        // std::cerr<<((name_var*)(((ValueExpr*)args[0])->val))->name<<"\n";
                        if(!args[0]->is_var()){
                            parser->error(location,"can only assign value for variables.");
                        }
                        value_type *lv=args[0]->eval(out);
                        auto it=args[1]->eval(out);
                        if(it->is_const()){
                            if(it->get_const())
                                out<<"MOVD "<<lv->get_addr()<<" "<<it->get_const()<<"\n";
                            else
                                out<<"CLR "<<lv->get_addr()<<"\n";
                        }
                        else if(it->get_addr()!=lv->get_addr()){
                            out<<"MOV "<<lv->get_addr()<<" "<<it->get_addr()<<"\n";
                        }
                        return lv;
                    }
                    if(opt!="=="&&opt!="!="&&opt!=">="&&opt!="<="&&opt.s.back()=='='){
                        string s=opt.s;
                        s.pop_back();
                        auto it=new Opt(parser,location,opt_type(s),args);
                        auto v=it->eval(out);
                        if(!args[0]->is_var()){
                            parser->error(location,"can only assign value for variables.");
                        }
                        auto lv=args[0]->eval(out);
                        if(v->is_const()){
                            out<<"MOVD "<<lv->get_addr()<<" "<<v->get_const()<<"\n";
                        }
                        else{
                            out<<"MOV "<<lv->get_addr()<<" "<<v->get_addr()<<"\n";
                        }
                        return lv;
                    }
                    auto it0=args[0]->eval(out);
                    if(!it0->is_const())it0->push(out);
                    auto it1=args[1]->eval(out);
                    if(it0->is_const()&&it1->is_const()){
                        return calc_bin(parser,location,opt.s,it0,it1);
                    }
                    if(it1->is_const()){
                        out<<"MOVD "<<B.get_addr()<<" "<<it1->get_const()<<"\n";
                    }
                    else if(it1->get_addr()!=B.get_addr()){
                        out<<"MOV "<<B.get_addr()<<" "<<it1->get_addr()<<"\n";
                    }
                    if(it0->is_const()){
                        out<<"MOVD "<<A.get_addr()<<" "<<it0->get_const()<<"\n";
                    }
                    else{
                        A.pop(out);
                    }
                    if(opt=="+"){
                        out<<"ADD_AB"<<"\n";
                        return &A;
                    }
                    if(opt=="-"){
                        out<<"SUB_AB\n";
                        return &A;
                    }
                    if(opt=="*"){
                        out<<"MUL_AB\n";
                        return &A;
                    }
                    if(opt=="/"||opt=="%"){
                        out<<"DIV_AB\n";
                        return opt=="/"?&A:&B;
                    }
                    if(opt=="|"||opt=="||"){
                        out<<"OR_AB"<<"\n";
                        return &A;
                    }
                    if(opt=="&"||opt=="&&"){
                        out<<"AND_AB"<<"\n";
                        return &A;
                    }
                    if(opt=="^"){
                        out<<"XOR_AB"<<"\n";
                        return &A;
                    }
                    if(opt==">>"){
                        out<<"SHR_AB"<<"\n";
                        return &A;
                    }
                    if(opt=="<<"){
                        out<<"SHL_AB"<<"\n";
                        return &A;
                    }
                    if(opt=="<"||opt==">"){
                        if(opt=="<")
                            out<<"SUB_AB"<<"\n";
                        else 
                            out<<"SUB_AB"<<"\n";
                        out<<"CLR "<<A.get_addr()<<"\n";
                        out<<"JNC __CMP_"<<id<<"\n";
                        out<<"INC "<<A.get_addr()<<"\n";
                        out<<"label __CMP_"<<id<<"\n";
                        return &A;
                    }
                    if(opt==">="||opt=="<="){
                        if(opt==">=")
                            out<<"SUB_AB"<<"\n";
                        else
                            out<<"SUB_AB"<<"\n";
                        out<<"CLR "<<A.get_addr()<<"\n";
                        out<<"JC __CMP_"<<id<<"\n";
                        out<<"INC "<<A.get_addr()<<"\n";
                        out<<"label __CMP_"<<id<<"\n";
                        return &A;
                    }
                    if(opt=="=="||opt=="!="){
                        out<<"SUB_AB"<<"\n";
                        out<<"CLR "<<B.get_addr()<<"\n";
                        if(opt=="==")
                            out<<"JZ "<<A.get_addr()<<" __CMP_"<<id<<"\n";
                        else
                            out<<"JNZ "<<A.get_addr()<<" __CMP_"<<id<<"\n";
                        out<<"INC "<<B.get_addr()<<"\n";
                        out<<"label __CMP_"<<id<<"\n";
                        return &B;
                    }
                }
                else if(opt=="~"){
                    auto it=args[0]->eval(out);
                    if(it->is_const())return new const_value_type(~it->get_const());
                    if(it->get_addr()!=A.get_addr()&&it->get_addr()!=B.get_addr()){
                        out<<"MOV "<<A.get_addr()<<" "<<it->get_addr()<<"\n";
                        it=&A;
                    }
                    out<<"CPL "<<it->get_addr()<<"\n";
                    return it;
                }
                else if(opt=="!"){
                    auto it=args[0]->eval(out);
                    if(it->is_const()) return new const_value_type(!it->get_const());
                    var_type *tar=&A;
                    if(it->get_addr()==tar->get_addr())tar=&B;
                    out<<"CLR "<<tar->get_addr()<<"\n";
                    out<<"JNZ "<<it->get_addr()<<" __NOT_NOSET_"<<id<<"\n";
                    out<<"INC "<<tar->get_addr()<<"\n";
                    out<<"label __NOT_NOSET_"<<id<<"\n";
                    return tar;
                }
                else if(opt=="-"){
                    auto it=args[0]->eval(out);
                    if(it->is_const())return new const_value_type(-it->get_const());
                    out<<"MOV "<<A.get_addr()<<" "<<it->get_addr()<<"\n";
                    out<<"CPL "<<A.get_addr()<<"\n";
                    out<<"INC "<<A.get_addr()<<"\n";
                    return &A;
                }
                else if(opt=="+"){
                    return args[0]->eval(out);
                }
                else if(opt=="++"){
                    auto it=args[0]->eval(out);
                    // if(it->is_const())return const_value_type(it->get_const()+1);
                    out<<"INC "<<it->get_addr()<<"\n";
                    return it;
                }
                else if(opt=="--"){
                    auto it=args[0]->eval(out);
                    // if(it->is_const())return const_value_type(it->get_const()-1);
                    out<<"DEC "<<it->get_addr()<<"\n";
                    return it;
                }
                return &var_void;
            }
        };
        struct ReturnExpr: public Expr{
            Expr* ret;
            ReturnExpr(Parser *parser, location_t location, Expr* ret):Expr(parser,location),ret(ret){}
            bool ends_with_ret()const{
                return true;
            }
            value_type* eval(ostream &out){
                if(ret!=nullptr){
                    auto arg_var=ret->eval(out);
                    auto eq_cmd=new Opt(parser,location,opt_type("="),
                        {new ValueExpr(parser,location,&A), 
                        new ValueExpr(parser,location,arg_var)});
                    eq_cmd->eval(out);
                }
                if(env == "main") out << "EXIT\n";
                else out << "RET\n";
                return &A;
            }
        };
        struct GotoExpr: public Expr{
            string s;
            GotoExpr(Parser *parser, location_t location, string s):Expr(parser,location),s(s){}
            value_type* eval(ostream &out){
                out<<"JMP "<<s<<"\n";
                return &var_void;
            }
        };

        class ExprSet: public Expr{
        public:
            vector<Expr*> args;
            ExprSet(Parser *parser, location_t location, vector<Expr*> args)
            :Expr(parser,location),args(args){}
            value_type* eval(ostream &out){
                for(int i=0;i<(int)args.size();){
                    auto it=args[i]->eval(out);
                    i++;
                    if(i==args.size()) return it;
                }
                return &var_void;
            }
            bool ends_with_ret()const{
                if(args.empty())return false;
                return args.back()->ends_with_ret();
            }
        };

        class LabelExpr: public Expr{
        public:
            string name;
            LabelExpr(Parser *parser, location_t location, string name)
            :Expr(parser,location),name(name){}
            value_type* eval(ostream &out){
                out <<"label "<<name<<"\n";
                return &var_void;
            }
        };

        class FlowExpr: public Expr{
        public:
            string id;
            Expr *IF,*THEN,*ELSE;
            FlowExpr(Parser *parser,location_t location,Expr* _IF,Expr* _THEN, Expr* _ELSE):Expr(parser,location){
                id=to_str(location.first_line,location.first_column);
                IF=_IF;
                THEN=_THEN;
                ELSE=_ELSE;
            }
            value_type* eval(ostream &out){
                auto it=IF->eval(out);
                if(it->is_const()){
                    if(it->get_const()){
                        THEN->eval(out);
                    }
                    else{
                        ELSE->eval(out);
                    }
                    return &var_void;
                }
                out << "JZ " << it->get_addr() << " __ELSE_"<<id << "\n";
                THEN->eval(out);
                out << "JMP " << "__END_"<<id << "\n";
                out << "label " << "__ELSE_"<<id <<"\n";
                ELSE->eval(out);
                out << "label " << "__END_"<<id << "\n";
                return &var_void;
            }
        };

        struct WhileExpr: public Expr{
            string id;
            Expr *WHILE,*DO;
            WhileExpr(Parser *parser, location_t location,Expr *WHILE, Expr *DO)
            :Expr(parser,location),id(to_str(location.first_line,location.first_column)),WHILE(WHILE),DO(DO){}
            value_type* eval(ostream &out){
                const string begin="__BEGIN_WHILE_"+id;
                const string end="__END_WHILE_"+id;
                out << "label "<<begin<<"\n";
                auto it=WHILE->eval(out);
                if(it->is_const()){
                    if(!it->get_const()){
                        out << "JMP "<<end<<"\n";
                    }
                }
                else{
                    out<<"JZ "<<it->get_addr()<<" "<<end<<"\n";
                }
                loop_continue.push_back(begin);
                loop_break.push_back(end);
                DO->eval(out);
                loop_continue.pop_back();
                loop_break.pop_back();
                out << "JMP "<<begin<<"\n";
                out << "label "<<end<<"\n";
                return &var_void;
            }
        };

        struct DoWhileExpr: public Expr{
            string id;
            Expr *DO,*WHILE;
            DoWhileExpr(Parser *parser,location_t location,Expr *DO,Expr *WHILE)
            :Expr(parser,location),id(to_str(location.first_line,location.first_column)),DO(DO),WHILE(WHILE){}
            value_type* eval(ostream &out){
                const string begin="__BEGIN_DOWHILE_"+id;
                const string check="__CHECK_DOWHILE_"+id;
                const string end="__END_DOWHILE_"+id;

                out << "label "<<begin<<"\n";

                loop_continue.push_back(check);
                loop_break.push_back(end);
                DO->eval(out);
                loop_continue.pop_back();
                loop_break.pop_back();

                out << "label "<<check<<'\n';
                auto it=WHILE->eval(out);
                if(it->is_const()){
                    if(it->get_const()){
                        out<< "JMP "<<begin<<"\n";
                    }
                }
                else{
                    out<<"JNZ "<<it->get_addr()<<" "<<begin<<"\n";
                }
                out << "label "<<end<<"\n";
                return &var_void;
            }
        };

        struct ForExpr: public Expr{
            string id;
            Expr *a,*b,*c,*S;
            ForExpr(Parser *parser,location_t location, Expr *a,Expr *b,Expr *c,Expr *S)
            :Expr(parser,location),id(to_str(location.first_line,location.first_column)),
            a(a),b(b),c(c),S(S){}
            value_type *eval(ostream &out){
                const string begin="__BEGIN_FOR_"+id;
                const string pp="__PLUSPLUS_FOR_"+id;
                const string end="__END_FOR_"+id;
                if(a!=nullptr) a->eval(out);
                out<<"label "<<begin<<'\n';
                value_type *it=b==nullptr?new const_value_type(1):b->eval(out);
                if(it->is_const()){
                    if(!it->get_const()){
                        out<<"JMP "<<end<<"\n";
                    }
                }
                else{
                    out<<"JZ "<<it->get_addr()<<" "<<end<<"\n";
                }
                loop_continue.push_back(pp);
                loop_break.push_back(end);
                S->eval(out);
                loop_continue.pop_back();
                loop_break.pop_back();
                out<<"label "<<pp<<'\n';
                if(c!=nullptr) c->eval(out);
                out<<"JMP "<<begin<<"\n";
                out<<"label "<<end<<"\n";
                return &var_void;
            }
        };

        struct ContinueExpr: public Expr{
            ContinueExpr(Parser *parser,location_t location)
            :Expr(parser,location){}
            value_type *eval(ostream &out){
                if(loop_continue.empty()){
                    parser->error(location,"continue not in a loop!");
                    return &var_void;
                }
                out<<"JMP "<<loop_continue.back()<<"\n";
                return &var_void;
            }
        };
        struct BreakExpr: public Expr{
            BreakExpr(Parser *parser,location_t location)
            :Expr(parser,location){}
            value_type *eval(ostream &out){
                if(loop_break.empty()){
                    parser->error(location,"break not in a loop!");
                    return &var_void;
                }
                out<<"JMP "<<loop_break.back()<<"\n";
                return &var_void;
            }
        };

        void FnDef(ostream &out,Parser *parser, location_t location, 
            int_type ret_type, string name, vector<int_type> types, vector<name_var> syms, vector<addr_type> addrs,
            vector<pair<string,addr_type>> defs,vector<Expr*> cmds)
        {
            if(fn_table.count(name)){
                auto &fn=fn_table[name];
                if(fn.defined){
                    parser->error(location, "Redefinition of function "+name+".");
                    return;
                }
                if(fn.ret_type != ret_type || fn.types != types || fn.addrs != addrs){
                    // std::cerr<<fn.types.size()<<":"<<types.size()<<"\n";
                    parser->error(location, "Ambiguous declarion of function "+name+".");
                    return;
                }
            }
            auto &fn=fn_table[name];
            fn.defined = true;
            fn.ret_type = ret_type;
            fn.types = types;
            fn.syms = syms;
            fn.addrs = addrs;
            fn.defs=defs;

            vector<pair<string,int>> override_var;

            for(int i=0;i<(int)syms.size();i++){
                if(var_addr.count(syms[i].name)){
                    override_var.emplace_back(syms[i].name,var_addr[syms[i].name]);
                }
                var_addr[syms[i].name]=addrs[i];
            }
            for(auto [name,addr]:defs){
                if(var_addr.count(name)){
                    override_var.emplace_back(name,var_addr[name]);
                }
                var_addr[name]=addr;
            }
            
            cmds.insert(cmds.begin(),new LabelExpr(parser,location,fn_label_prefix+name));
            env=name;
            if(cmds.empty()||!cmds.back()->ends_with_ret()){
                // cout<<cmds.size()<<' ';
                // if(!cmds.empty()){
                //     cout<<cmds.back()->ends_with_ret()<<' ';
                // }
                // cout<<'\n';
                cmds.push_back(new ReturnExpr(parser,location,nullptr));
            }
            for(auto cmd:cmds){
                cmd->eval(out);
            }

            for(int i=0;i<(int)syms.size();i++){
                var_addr.erase(syms[i].name);
            }
            for(auto [name,addr]:defs){
                var_addr.erase(name);
            }
            for(auto [sym_name, addr]:override_var){
                var_addr[sym_name]=addr;
            }
        }
        struct FnCall: public Expr{
            string name;
            vector<Expr*> args;   //actual arguments
            FnCall(Parser *parser, location_t location, string name, vector<Expr*> args)
            : Expr(parser,location), name(name),args(args) {
            }
            ~FnCall(){
                for(auto &p:args) delete p;
            }
            value_type* eval(ostream &out){
                if(name=="main"){
                    parser->error(location,"main() can only be called by system!");
                    return &var_void;
                }
                if(name=="__raw"){
                    for(auto arg:args){
                        auto it=arg->eval(out);
                        if(!it->is_const()){
                            parser->error(location,"__raw called with non_constant argument(s).");
                        }
                        out<<it->get_const()<<" ";
                    }
                    out<<"\n";
                    return &var_void;
                }
                if(name=="__addr"){
                    if(args.size()!=1||!args[0]->is_var()||((ValueExpr*)args[0])->val->is_const()){
                        parser->error(location,"__addr must be called with exactly one variable.");
                        return &var_void;
                    }
                    return new const_value_type(((ValueExpr*)args[0])->val->get_addr());
                }
                if(name=="__var"){
                    if(args.size()!=1||!(args[0]->eval(out)->is_const())){
                        parser->error(location,"__var must be called with exactly one constant.");
                        return &var_void;
                    }
                    return new addr_var(args[0]->eval(out)->get_const());
                }
                // if(name=="__is_constexpr"){
                //     if(args.size()!=1){
                //         parser->error(location,"__is_constexpr must be called with exactly one expression.");
                //         return &var_void;
                //     }
                //     auto it=args[0]->eval(out);
                //     return new const_value_type(it->is_const());
                // }
                if(!fn_table.count(name)){
                    parser->error(location,"Undeclared function "+name+".");
                    return &var_void;
                }
                auto &fn=fn_table[name];
                if(args.size()!=fn.types.size()){
                    std::stringstream ss;
                    ss<<"Wrong number of arguments for function "<<name<<", ";
                    ss<<"expected "<<fn.types.size()<<", ";
                    ss<<"found "<<args.size()<<".";
                    parser->error(location, ss.str());
                    return &var_void;
                }
                push_env(out);
                for(int i=0;i<(int)fn.addrs.size();i++){
                    auto arg_var=args[i]->eval(out);
                    // auto it=new Opt(parser,location,opt_type("="),
                    //     {new ValueExpr(parser,location,new addr_var(fn.addrs[i])),
                    //      new ValueExpr(parser,location,arg_var)});
                    // it->eval(out);
                    if(arg_var->is_const()){
                        if(arg_var->get_const())
                            out<<"MOVD "<<fn.addrs[i]<<" "<<arg_var->get_const()<<"\n";
                        else
                            out<<"CLR "<<fn.addrs[i]<<"\n";
                    }
                    else if(fn.addrs[i] != arg_var ->get_addr()){
                        out<<"MOV "<<fn.addrs[i]<<" "<<arg_var->get_addr()<<"\n";
                    }
                }
                out << "CALL "<<fn_label_prefix<< name << "\n";
                pop_env(out);
                return &A;
            }
        };
    }
    
    #define YYLLOC_DEFAULT(Cur, Rhs, N) calcLocation(Cur, Rhs, N)
    #define yylex lexer.yylex
    #define pl this,yylhs.location

    #undef get_addr
    #undef push
    #undef pop
    #undef push_env
    #undef pop_env
    
    #define get_addr() gaddr(pl)
}

/* %token HELLO
%token WORLD */
%token Int_Constant Identifier 
%token Type
%token IF ELSE WHILE DO RETURN GOTO CONTINUE BREAK FOR
%token ADD2 "++"
%token SUB2 "--"
%token Add_eq "+="
%token Sub_eq "-=";
%token Mul_eq "*="
%token Div_eq "/="
%token Mod_eq "%="
%token RL_eq "<<="
%token RR_eq ">>="
%token And_eq "&="
%token Xor_eq "^="
%token Or_eq "|="
%token Logic_and "&&"
%token Logic_or "||"
%token RL "<<"
%token RR ">>"
%token EQ "=="
%token NE "!="
%token LE "<="
%token GE ">="



%right THEN ELSE
//todo: priority
%left ','
%right '=' "+=" "-=" "*=" "/=" "%=" "<<=" ">>=" "&=" "^=" "|="
%right U_OPT3 '?' ':'
%left "||"
%left "&&"
%left '|'
%left '^'
%left '&'
%left "<<" ">>" 
%left "==" "!="
%left '<' "<=" '>' ">="
%left '+' '-'
%left '*' '/' '%'
%right '!' '~'
%right U_pos U_neg U_LADD U_LSUB 
/* %left U_RADD U_RSUB */

%expect 0

%start P

%%

/* prog: hello_world
    | prog hello_world

hello_world: HELLO WORLD '!' { 
    out << "Goodbye " << $WORLD << '!' << std::endl; 
} */

P   :               {
        out <<"JMP "<<fn_label_prefix<<"main\n";
    }
    |   P Type Identifier '=' E ';' {
            auto it=$5.ast->eval(out);
            if(it->is_const()){
                it=new addr_var(it->get_const());
            }
            else if(!$5.ast->is_var()){
                error(yylhs.location,"Defined global variable with neither an address nor a varible.");
            }
            // std::cerr<<$3.str<<"="<<it->get_addr()<<"\n";
            var_addr[$3.str]=it->get_addr();
        }
    |   P fn_dec    
    |   P fn_def
    ;

SS  :               {$$.ast=new ExprSet(pl,{});}
    |   SS S        {$$.ast=$1.ast;((ExprSet*)$1.ast)->args.push_back($2.ast);}
    ;

E_null: /*empty*/    {$$.ast=nullptr;}
      | E            {$$.ast=$E.ast;}

S   :   ';'                                 {$$.ast=new ExprSet(pl,{});}
    |   E ';'                               {$$.ast=$1.ast;}
    |   '{' SS '}'                          {$$.ast=$2.ast;}
    |   IF '(' E ')'  S ELSE S %prec ELSE   {$$.ast=new FlowExpr(pl,$3.ast,$5.ast,$7.ast);}
    |   IF '(' E ')'  S        %prec THEN   {$$.ast=new FlowExpr(pl,$3.ast,$5.ast,new ExprSet(pl,{}));}
    |   WHILE '(' E ')' S                   {$$.ast=new WhileExpr(pl,$3.ast,$5.ast);}
    |   WHILE '(' ')' S                     {$$.ast=new WhileExpr(pl,new ValueExpr(pl,new const_value_type(1)),$4.ast);}
    |   DO S WHILE '(' E ')' ';'            {$$.ast=new DoWhileExpr(pl,$2.ast,$5.ast);}
    |   Identifier ':'          {$$.ast=new LabelExpr(pl,$1.str);} 
    |   GOTO Identifier ';'     {$$.ast=new GotoExpr(pl,$2.str);}
    |   RETURN ';'              {$$.ast=new ReturnExpr(pl,nullptr);}
    |   RETURN E ';'            {$$.ast=new ReturnExpr(pl,$2.ast);}
    |   CONTINUE ';'            {$$.ast=new ContinueExpr(pl);}
    |   BREAK ';'               {$$.ast=new BreakExpr(pl);}
    |   FOR '(' E_null ';' E_null ';' E_null ')' S {$$.ast=new ForExpr(pl,$3.ast,$5.ast,$7.ast,$9.ast);}
    ;

/* C   :   Int_Constant
    |   C '+' C
    |   C '-' C
    |   C '*' C
    |   C '/' C
    |   C '%' C
    |   '-' C %prec U_neg
    ; */

E   :   '!' E       {$$.ast=new Opt(pl,opt_type($1.str),{$2.ast});}
    |   '~' E       {$$.ast=new Opt(pl,opt_type($1.str),{$2.ast});}
    |   '-' E %prec U_neg   {$$.ast=new Opt(pl,opt_type($1.str),{$2.ast});}
    |   '+' E %prec U_pos   {$$.ast=new Opt(pl,opt_type($1.str),{$2.ast});}
    |   "++" Identifier %prec U_LADD {$$.ast=new Opt(pl,opt_type($1.str),{new ValueExpr(pl,new name_var($2.str))});}
    |   "--" Identifier %prec U_LSUB {$$.ast=new Opt(pl,opt_type($1.str),{new ValueExpr(pl,new name_var($2.str))});}
    |   E '*' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '/' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '%' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '+' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '-' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "<<" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E ">>" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '<' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "<=" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '>' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E ">=" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "==" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "!=" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '&' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '^' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '|' E     {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "&&" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "||" E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E '?' E ':' E %prec U_OPT3  {$$.ast=new Opt(pl,opt_type("?:"),{$1.ast,$3.ast,$5.ast});}
    |   E '=' E    {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "+=" E   {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "-=" E   {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "*=" E   {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "/=" E   {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   E "%=" E   {$$.ast=new Opt(pl,opt_type($2.str),{$1.ast,$3.ast});}
    |   '(' E ')'           {$$.ast = $2.ast;}
    |   Int_Constant        {
            // out << "pushd "<<$1.str<<'\n';
            int x=0;
            if($1.str.size()>=2&&$1.str[1]=='x'){
                for(int i=2;i<$1.str.size();i++){
                    int cur;
                    char c=$1.str[i];
                    if('a'<=c&&c<='z')c=c-'a'+'A';
                    if('0'<=c&&c<='9') cur=c-'0';
                    else cur=c-'A'+10;
                    x=x*16+cur;
                }
            }
            else if($1.str.size()>=2&&$1.str[1]=='b'){
                for(int i=2;i<$1.str.size();i++){
                    int cur;
                    char c=$1.str[i];
                    if('a'<=c&&c<='z')c=c-'a'+'A';
                    if('0'<=c&&c<='9') cur=c-'0';
                    else cur=c-'A'+10;
                    x=x*2+cur;
                }
            }
            else{
                std::stringstream ss;
                ss<<$1.str;
                ss>>x;
            }
            $$.ast=new ValueExpr(pl,new const_value_type(x));
        }
    |   Identifier          {
            // cout << "push "<<$1.str<<'\n';
            $$.ast=new ValueExpr(pl,new name_var($1.str));
        }
    |   Identifier '(' ')'{
            // out << "call "<<$1.str<<"\n";
            $$.ast=new FnCall(pl,$1.str,{});
        }
    |   Identifier '(' Elist ')'    {
            // out << "call "<<$1.str<<"\n";
            $$.ast=new FnCall(pl,$1.str,((ExprSet*)$3.ast)->args);
        }
    ;
 
Elist   :   E           {$$.ast=new ExprSet(pl,{$1.ast});}
        |   Elist ',' E {$$.ast=$1.ast;((ExprSet*)$1.ast)->args.push_back($3.ast);}
        ;

/* typelist:   Type    
            {$$.ast=new Typelist(pl,{get_int_type($1.str)});}
        |   typelist ',' Type
            {
                $$.ast=$1.ast;
                ((Typelist*)$$.ast)->types.push_back(get_int_type($3.str));
            }
        ;  */

deflist :   Type Identifier '=' E   {
                auto it=$4.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!$E.ast->is_var()){
                    error(yylhs.location,"address must be a constant/variable.");
                    return 22;
                }
                $$.ast=new Deflist(pl,
                    {get_int_type($Type.str)},
                    {name_var($2.str)},
                    {it->get_addr()});
            }
        |   deflist ',' Type Identifier '=' E {
                auto it=$6.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!$E.ast->is_var()){
                    error(yylhs.location,"address must be a constant/follow a variable.");
                    return 22;
                }
                $$.ast=$1.ast;
                ((Deflist*)$$.ast)->types.push_back(get_int_type($Type.str));
                ((Deflist*)$$.ast)->syms.push_back(name_var($4.str));
                ((Deflist*)$$.ast)->addrs.push_back(it->get_addr());
            }
        ;

def     :   Type Identifier '=' E       {
                auto it=$4.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!$4.ast->is_var()){
                    error(yylhs.location,"address must be a constant/follow a variable.");
                }
                $$.defs={{$2.str,it->get_addr()}};
            }
        |   def ',' Identifier '=' E {
                $$.defs=$1.defs;
                $1.defs.clear();
                auto it=$5.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!$5.ast->is_var()){
                    error(yylhs.location,"address must be a constant/follow a variable.");
                }
                $$.defs.push_back({$3.str,it->get_addr()});
            }
        ;

fn_dec  :   Type Identifier '(' ')' ';'     
            {
                // out << "fn_dec "<<$2.str<<"\n";
                FnDec(pl,get_int_type($1.str),$2.str,{},{});
            }
        |   Type Identifier '(' deflist ')' ';'       
            {
                // out << "fn_dec "<<$2.str<<"\n";
                FnDec(pl,get_int_type($Type.str),$2.str,((Deflist*)$deflist.ast)->types,((Deflist*)$deflist.ast)->addrs);
            }
        ;

fn_def  :   Type Identifier '(' ')' '{' SS '}'
            {
                // out << "fn_def "<<$2.str<<"\n";
                FnDef(out,pl,get_int_type($1.str),
                $2.str,{},{},{},
                {},{$6.ast});
            }
        |   Type Identifier '(' deflist ')' '{' SS '}' 
            {
                // out << "fn_def "<<$2.str<<"\n";
                Deflist *it=(Deflist*) $4.ast;
                FnDef(out,pl,get_int_type($1.str),
                $2.str,it->types,it->syms,it->addrs,
                {},{$7.ast});
            }
        |   Type Identifier '(' ')' '{' def ';' SS '}'
            {
                // out << "fn_def "<<$2.str<<"\n";
                FnDef(out,pl,get_int_type($1.str),
                $2.str,{},{},{},
                $def.defs,{$6.ast});
            }
        |   Type Identifier '(' deflist ')' '{' def ';' SS '}' 
            {
                // out << "fn_def "<<$2.str<<"\n";
                Deflist *it=(Deflist*) $4.ast;
                FnDef(out,pl,get_int_type($1.str),
                $2.str,it->types,it->syms,it->addrs,
                $def.defs,{$SS.ast});
            }
        ;
%%

namespace foo
{
    template<typename RHS>
    inline void calcLocation(location_t &current, const RHS &rhs, const std::size_t n)
    {
        current = location_t(YYRHSLOC(rhs, 1).first_line, YYRHSLOC(rhs, 1).first_column,
                             YYRHSLOC(rhs, n).last_line,  YYRHSLOC(rhs, n).first_column);
    }
    
    void FooBisonParser::error(const location_t &location, const std::string &message)
    {
        std::cerr << "Error at " << location << ": " << message << std::endl;
    }
}