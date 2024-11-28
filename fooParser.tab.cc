// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "fooParser.tab.hh"


// Unqualified %code blocks.
#line 40 "fooParser.yy"

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

        // class Label: public Expr{
        // public:
        //     string label;
        //     Label(Parser *parser, location_t loc, string l)
        //     :Expr(parser,loc),label(l),rhs(r){}
        //     value_type *eval(ostream &out){
        //         out<<"label "<<label<<"\n";
        //         return &var_void;
        //     }
        // }

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
                            out<<"CMP_AB"<<"\n";
                        else 
                            out<<"CMP_BA"<<"\n";
                        out<<"CLR "<<A.get_addr()<<"\n";
                        out<<"JNC __CMP_"<<id<<"\n";
                        out<<"INC "<<A.get_addr()<<"\n";
                        out<<"label __CMP_"<<id<<"\n";
                        return &A;
                    }
                    if(opt==">="||opt=="<="){
                        if(opt==">=")
                            out<<"CMP_AB"<<"\n";
                        else
                            out<<"CMP_BA"<<"\n";
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
                            out<<"JNZ "<<A.get_addr()<<" __CMP_"<<id<<"\n";
                        else
                            out<<"JZ "<<A.get_addr()<<" __CMP_"<<id<<"\n";
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

#line 899 "fooParser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 17 "fooParser.yy"
namespace foo {
#line 992 "fooParser.tab.cc"

  /// Build a parser object.
  FooBisonParser::FooBisonParser (FooLexer &lexer_yyarg, const bool debug_yyarg, std::ostream &out_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      debug (debug_yyarg),
      out (out_yyarg)
  {}

  FooBisonParser::~FooBisonParser ()
  {}

  FooBisonParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  FooBisonParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  FooBisonParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  FooBisonParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}


  template <typename Base>
  FooBisonParser::symbol_kind_type
  FooBisonParser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  FooBisonParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  FooBisonParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_kind.
  FooBisonParser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  FooBisonParser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  FooBisonParser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  FooBisonParser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  FooBisonParser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  FooBisonParser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  FooBisonParser::symbol_kind_type
  FooBisonParser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  FooBisonParser::symbol_kind_type
  FooBisonParser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  FooBisonParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  FooBisonParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  FooBisonParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  FooBisonParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  FooBisonParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  FooBisonParser::symbol_kind_type
  FooBisonParser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  FooBisonParser::stack_symbol_type::stack_symbol_type ()
  {}

  FooBisonParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  FooBisonParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  FooBisonParser::stack_symbol_type&
  FooBisonParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  FooBisonParser::stack_symbol_type&
  FooBisonParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  FooBisonParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  FooBisonParser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  FooBisonParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  FooBisonParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  FooBisonParser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  FooBisonParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  FooBisonParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  FooBisonParser::debug_level_type
  FooBisonParser::debug_level () const
  {
    return yydebug_;
  }

  void
  FooBisonParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  FooBisonParser::state_type
  FooBisonParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  FooBisonParser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  FooBisonParser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  FooBisonParser::operator() ()
  {
    return parse ();
  }

  int
  FooBisonParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 34 "fooParser.yy"
{
    #if YYDEBUG != 0
        set_debug_level(debug);
    #endif
}

#line 1340 "fooParser.tab.cc"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // P: %empty
#line 953 "fooParser.yy"
                    {
        out <<"JMP "<<fn_label_prefix<<"main\n";
    }
#line 1480 "fooParser.tab.cc"
    break;

  case 3: // P: P Type Identifier '=' E ';'
#line 956 "fooParser.yy"
                                    {
            auto it=yystack_[1].value.ast->eval(out);
            if(it->is_const()){
                it=new addr_var(it->get_const());
            }
            else if(!yystack_[1].value.ast->is_var()){
                error(yylhs.location,"Defined global variable with neither an address nor a varible.");
            }
            // std::cerr<<$3.str<<"="<<it->get_addr()<<"\n";
            var_addr[yystack_[3].value.str]=it->get_addr();
        }
#line 1496 "fooParser.tab.cc"
    break;

  case 6: // SS: %empty
#line 971 "fooParser.yy"
                    {yylhs.value.ast=new ExprSet(pl,{});}
#line 1502 "fooParser.tab.cc"
    break;

  case 7: // SS: SS S
#line 972 "fooParser.yy"
                    {yylhs.value.ast=yystack_[1].value.ast;((ExprSet*)yystack_[1].value.ast)->args.push_back(yystack_[0].value.ast);}
#line 1508 "fooParser.tab.cc"
    break;

  case 8: // E_null: %empty
#line 975 "fooParser.yy"
                     {yylhs.value.ast=nullptr;}
#line 1514 "fooParser.tab.cc"
    break;

  case 9: // E_null: E
#line 976 "fooParser.yy"
                     {yylhs.value.ast=yystack_[0].value.ast;}
#line 1520 "fooParser.tab.cc"
    break;

  case 10: // S: ';'
#line 978 "fooParser.yy"
                                            {yylhs.value.ast=new ExprSet(pl,{});}
#line 1526 "fooParser.tab.cc"
    break;

  case 11: // S: E ';'
#line 979 "fooParser.yy"
                                            {yylhs.value.ast=yystack_[1].value.ast;}
#line 1532 "fooParser.tab.cc"
    break;

  case 12: // S: '{' SS '}'
#line 980 "fooParser.yy"
                                            {yylhs.value.ast=yystack_[1].value.ast;}
#line 1538 "fooParser.tab.cc"
    break;

  case 13: // S: IF '(' E ')' S ELSE S
#line 981 "fooParser.yy"
                                            {yylhs.value.ast=new FlowExpr(pl,yystack_[4].value.ast,yystack_[2].value.ast,yystack_[0].value.ast);}
#line 1544 "fooParser.tab.cc"
    break;

  case 14: // S: IF '(' E ')' S
#line 982 "fooParser.yy"
                                            {yylhs.value.ast=new FlowExpr(pl,yystack_[2].value.ast,yystack_[0].value.ast,new ExprSet(pl,{}));}
#line 1550 "fooParser.tab.cc"
    break;

  case 15: // S: WHILE '(' E ')' S
#line 983 "fooParser.yy"
                                            {yylhs.value.ast=new WhileExpr(pl,yystack_[2].value.ast,yystack_[0].value.ast);}
#line 1556 "fooParser.tab.cc"
    break;

  case 16: // S: WHILE '(' ')' S
#line 984 "fooParser.yy"
                                            {yylhs.value.ast=new WhileExpr(pl,new ValueExpr(pl,new const_value_type(1)),yystack_[0].value.ast);}
#line 1562 "fooParser.tab.cc"
    break;

  case 17: // S: DO S WHILE '(' E ')' ';'
#line 985 "fooParser.yy"
                                            {yylhs.value.ast=new DoWhileExpr(pl,yystack_[5].value.ast,yystack_[2].value.ast);}
#line 1568 "fooParser.tab.cc"
    break;

  case 18: // S: Identifier ':'
#line 986 "fooParser.yy"
                                {yylhs.value.ast=new LabelExpr(pl,yystack_[1].value.str);}
#line 1574 "fooParser.tab.cc"
    break;

  case 19: // S: GOTO Identifier ';'
#line 987 "fooParser.yy"
                                {yylhs.value.ast=new GotoExpr(pl,yystack_[1].value.str);}
#line 1580 "fooParser.tab.cc"
    break;

  case 20: // S: RETURN ';'
#line 988 "fooParser.yy"
                                {yylhs.value.ast=new ReturnExpr(pl,nullptr);}
#line 1586 "fooParser.tab.cc"
    break;

  case 21: // S: RETURN E ';'
#line 989 "fooParser.yy"
                                {yylhs.value.ast=new ReturnExpr(pl,yystack_[1].value.ast);}
#line 1592 "fooParser.tab.cc"
    break;

  case 22: // S: CONTINUE ';'
#line 990 "fooParser.yy"
                                {yylhs.value.ast=new ContinueExpr(pl);}
#line 1598 "fooParser.tab.cc"
    break;

  case 23: // S: BREAK ';'
#line 991 "fooParser.yy"
                                {yylhs.value.ast=new BreakExpr(pl);}
#line 1604 "fooParser.tab.cc"
    break;

  case 24: // S: FOR '(' E_null ';' E_null ';' E_null ')' S
#line 992 "fooParser.yy"
                                                   {yylhs.value.ast=new ForExpr(pl,yystack_[6].value.ast,yystack_[4].value.ast,yystack_[2].value.ast,yystack_[0].value.ast);}
#line 1610 "fooParser.tab.cc"
    break;

  case 25: // E: '!' E
#line 1005 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[0].value.ast});}
#line 1616 "fooParser.tab.cc"
    break;

  case 26: // E: '~' E
#line 1006 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[0].value.ast});}
#line 1622 "fooParser.tab.cc"
    break;

  case 27: // E: '-' E
#line 1007 "fooParser.yy"
                            {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[0].value.ast});}
#line 1628 "fooParser.tab.cc"
    break;

  case 28: // E: '+' E
#line 1008 "fooParser.yy"
                            {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[0].value.ast});}
#line 1634 "fooParser.tab.cc"
    break;

  case 29: // E: "++" Identifier
#line 1009 "fooParser.yy"
                                     {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{new ValueExpr(pl,new name_var(yystack_[0].value.str))});}
#line 1640 "fooParser.tab.cc"
    break;

  case 30: // E: "--" Identifier
#line 1010 "fooParser.yy"
                                     {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{new ValueExpr(pl,new name_var(yystack_[0].value.str))});}
#line 1646 "fooParser.tab.cc"
    break;

  case 31: // E: E '*' E
#line 1011 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1652 "fooParser.tab.cc"
    break;

  case 32: // E: E '/' E
#line 1012 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1658 "fooParser.tab.cc"
    break;

  case 33: // E: E '%' E
#line 1013 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1664 "fooParser.tab.cc"
    break;

  case 34: // E: E '+' E
#line 1014 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1670 "fooParser.tab.cc"
    break;

  case 35: // E: E '-' E
#line 1015 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1676 "fooParser.tab.cc"
    break;

  case 36: // E: E "<<" E
#line 1016 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1682 "fooParser.tab.cc"
    break;

  case 37: // E: E ">>" E
#line 1017 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1688 "fooParser.tab.cc"
    break;

  case 38: // E: E '<' E
#line 1018 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1694 "fooParser.tab.cc"
    break;

  case 39: // E: E "<=" E
#line 1019 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1700 "fooParser.tab.cc"
    break;

  case 40: // E: E '>' E
#line 1020 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1706 "fooParser.tab.cc"
    break;

  case 41: // E: E ">=" E
#line 1021 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1712 "fooParser.tab.cc"
    break;

  case 42: // E: E "==" E
#line 1022 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1718 "fooParser.tab.cc"
    break;

  case 43: // E: E "!=" E
#line 1023 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1724 "fooParser.tab.cc"
    break;

  case 44: // E: E '&' E
#line 1024 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1730 "fooParser.tab.cc"
    break;

  case 45: // E: E '^' E
#line 1025 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1736 "fooParser.tab.cc"
    break;

  case 46: // E: E '|' E
#line 1026 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1742 "fooParser.tab.cc"
    break;

  case 47: // E: E "&&" E
#line 1027 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1748 "fooParser.tab.cc"
    break;

  case 48: // E: E "||" E
#line 1028 "fooParser.yy"
                    {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1754 "fooParser.tab.cc"
    break;

  case 49: // E: E '?' E ':' E
#line 1029 "fooParser.yy"
                                    {yylhs.value.ast=new Opt(pl,opt_type("?:"),{yystack_[4].value.ast,yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1760 "fooParser.tab.cc"
    break;

  case 50: // E: E '=' E
#line 1030 "fooParser.yy"
                   {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1766 "fooParser.tab.cc"
    break;

  case 51: // E: E "+=" E
#line 1031 "fooParser.yy"
                   {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1772 "fooParser.tab.cc"
    break;

  case 52: // E: E "-=" E
#line 1032 "fooParser.yy"
                   {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1778 "fooParser.tab.cc"
    break;

  case 53: // E: E "*=" E
#line 1033 "fooParser.yy"
                   {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1784 "fooParser.tab.cc"
    break;

  case 54: // E: E "/=" E
#line 1034 "fooParser.yy"
                   {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1790 "fooParser.tab.cc"
    break;

  case 55: // E: E "%=" E
#line 1035 "fooParser.yy"
                   {yylhs.value.ast=new Opt(pl,opt_type(yystack_[1].value.str),{yystack_[2].value.ast,yystack_[0].value.ast});}
#line 1796 "fooParser.tab.cc"
    break;

  case 56: // E: '(' E ')'
#line 1036 "fooParser.yy"
                            {yylhs.value.ast = yystack_[1].value.ast;}
#line 1802 "fooParser.tab.cc"
    break;

  case 57: // E: Int_Constant
#line 1037 "fooParser.yy"
                            {
            // out << "pushd "<<$1.str<<'\n';
            int x=0;
            if(yystack_[0].value.str.size()>=2&&yystack_[0].value.str[1]=='x'){
                for(int i=2;i<yystack_[0].value.str.size();i++){
                    int cur;
                    char c=yystack_[0].value.str[i];
                    if('a'<=c&&c<='z')c=c-'a'+'A';
                    if('0'<=c&&c<='9') cur=c-'0';
                    else cur=c-'A'+10;
                    x=x*16+cur;
                }
            }
            else if(yystack_[0].value.str.size()>=2&&yystack_[0].value.str[1]=='b'){
                for(int i=2;i<yystack_[0].value.str.size();i++){
                    int cur;
                    char c=yystack_[0].value.str[i];
                    if('a'<=c&&c<='z')c=c-'a'+'A';
                    if('0'<=c&&c<='9') cur=c-'0';
                    else cur=c-'A'+10;
                    x=x*2+cur;
                }
            }
            else{
                std::stringstream ss;
                ss<<yystack_[0].value.str;
                ss>>x;
            }
            yylhs.value.ast=new ValueExpr(pl,new const_value_type(x));
        }
#line 1837 "fooParser.tab.cc"
    break;

  case 58: // E: Identifier
#line 1067 "fooParser.yy"
                            {
            // cout << "push "<<$1.str<<'\n';
            yylhs.value.ast=new ValueExpr(pl,new name_var(yystack_[0].value.str));
        }
#line 1846 "fooParser.tab.cc"
    break;

  case 59: // E: Identifier '(' ')'
#line 1071 "fooParser.yy"
                          {
            // out << "call "<<$1.str<<"\n";
            yylhs.value.ast=new FnCall(pl,yystack_[2].value.str,{});
        }
#line 1855 "fooParser.tab.cc"
    break;

  case 60: // E: Identifier '(' Elist ')'
#line 1075 "fooParser.yy"
                                    {
            // out << "call "<<$1.str<<"\n";
            yylhs.value.ast=new FnCall(pl,yystack_[3].value.str,((ExprSet*)yystack_[1].value.ast)->args);
        }
#line 1864 "fooParser.tab.cc"
    break;

  case 61: // Elist: E
#line 1081 "fooParser.yy"
                        {yylhs.value.ast=new ExprSet(pl,{yystack_[0].value.ast});}
#line 1870 "fooParser.tab.cc"
    break;

  case 62: // Elist: Elist ',' E
#line 1082 "fooParser.yy"
                        {yylhs.value.ast=yystack_[2].value.ast;((ExprSet*)yystack_[2].value.ast)->args.push_back(yystack_[0].value.ast);}
#line 1876 "fooParser.tab.cc"
    break;

  case 63: // deflist: Type Identifier '=' E
#line 1094 "fooParser.yy"
                                    {
                auto it=yystack_[0].value.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!yystack_[0].value.ast->is_var()){
                    error(yylhs.location,"address must be a constant/variable.");
                    return 22;
                }
                yylhs.value.ast=new Deflist(pl,
                    {get_int_type(yystack_[3].value.str)},
                    {name_var(yystack_[2].value.str)},
                    {it->get_addr()});
            }
#line 1895 "fooParser.tab.cc"
    break;

  case 64: // deflist: deflist ',' Type Identifier '=' E
#line 1108 "fooParser.yy"
                                              {
                auto it=yystack_[0].value.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!yystack_[0].value.ast->is_var()){
                    error(yylhs.location,"address must be a constant/follow a variable.");
                    return 22;
                }
                yylhs.value.ast=yystack_[5].value.ast;
                ((Deflist*)yylhs.value.ast)->types.push_back(get_int_type(yystack_[3].value.str));
                ((Deflist*)yylhs.value.ast)->syms.push_back(name_var(yystack_[2].value.str));
                ((Deflist*)yylhs.value.ast)->addrs.push_back(it->get_addr());
            }
#line 1914 "fooParser.tab.cc"
    break;

  case 65: // def: Type Identifier '=' E
#line 1124 "fooParser.yy"
                                        {
                auto it=yystack_[0].value.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!yystack_[0].value.ast->is_var()){
                    error(yylhs.location,"address must be a constant/follow a variable.");
                }
                yylhs.value.defs={{yystack_[2].value.str,it->get_addr()}};
            }
#line 1929 "fooParser.tab.cc"
    break;

  case 66: // def: def ',' Identifier '=' E
#line 1134 "fooParser.yy"
                                     {
                yylhs.value.defs=yystack_[4].value.defs;
                yystack_[4].value.defs.clear();
                auto it=yystack_[0].value.ast->eval(out);
                if(it->is_const()){
                    it=new addr_var(it->get_const());
                }
                else if(!yystack_[0].value.ast->is_var()){
                    error(yylhs.location,"address must be a constant/follow a variable.");
                }
                yylhs.value.defs.push_back({yystack_[2].value.str,it->get_addr()});
            }
#line 1946 "fooParser.tab.cc"
    break;

  case 67: // fn_dec: Type Identifier '(' ')' ';'
#line 1149 "fooParser.yy"
            {
                // out << "fn_dec "<<$2.str<<"\n";
                FnDec(pl,get_int_type(yystack_[4].value.str),yystack_[3].value.str,{},{});
            }
#line 1955 "fooParser.tab.cc"
    break;

  case 68: // fn_dec: Type Identifier '(' deflist ')' ';'
#line 1154 "fooParser.yy"
            {
                // out << "fn_dec "<<$2.str<<"\n";
                FnDec(pl,get_int_type(yystack_[5].value.str),yystack_[4].value.str,((Deflist*)yystack_[2].value.ast)->types,((Deflist*)yystack_[2].value.ast)->addrs);
            }
#line 1964 "fooParser.tab.cc"
    break;

  case 69: // fn_def: Type Identifier '(' ')' '{' SS '}'
#line 1161 "fooParser.yy"
            {
                // out << "fn_def "<<$2.str<<"\n";
                FnDef(out,pl,get_int_type(yystack_[6].value.str),
                yystack_[5].value.str,{},{},{},
                {},{yystack_[1].value.ast});
            }
#line 1975 "fooParser.tab.cc"
    break;

  case 70: // fn_def: Type Identifier '(' deflist ')' '{' SS '}'
#line 1168 "fooParser.yy"
            {
                // out << "fn_def "<<$2.str<<"\n";
                Deflist *it=(Deflist*) yystack_[4].value.ast;
                FnDef(out,pl,get_int_type(yystack_[7].value.str),
                yystack_[6].value.str,it->types,it->syms,it->addrs,
                {},{yystack_[1].value.ast});
            }
#line 1987 "fooParser.tab.cc"
    break;

  case 71: // fn_def: Type Identifier '(' ')' '{' def ';' SS '}'
#line 1176 "fooParser.yy"
            {
                // out << "fn_def "<<$2.str<<"\n";
                FnDef(out,pl,get_int_type(yystack_[8].value.str),
                yystack_[7].value.str,{},{},{},
                yystack_[3].value.defs,{yystack_[3].value.ast});
            }
#line 1998 "fooParser.tab.cc"
    break;

  case 72: // fn_def: Type Identifier '(' deflist ')' '{' def ';' SS '}'
#line 1183 "fooParser.yy"
            {
                // out << "fn_def "<<$2.str<<"\n";
                Deflist *it=(Deflist*) yystack_[6].value.ast;
                FnDef(out,pl,get_int_type(yystack_[9].value.str),
                yystack_[8].value.str,it->types,it->syms,it->addrs,
                yystack_[3].value.defs,{yystack_[1].value.ast});
            }
#line 2010 "fooParser.tab.cc"
    break;


#line 2014 "fooParser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  FooBisonParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  FooBisonParser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "Int_Constant", "Identifier",
  "Type", "IF", "ELSE", "WHILE", "DO", "RETURN", "GOTO", "CONTINUE",
  "BREAK", "FOR", "++", "--", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=",
  "&=", "^=", "|=", "&&", "||", "<<", ">>", "==", "!=", "<=", ">=", "THEN",
  "','", "'='", "U_OPT3", "'?'", "':'", "'|'", "'^'", "'&'", "'<'", "'>'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "U_pos", "U_neg",
  "U_LADD", "U_LSUB", "';'", "'{'", "'}'", "'('", "')'", "$accept", "P",
  "SS", "E_null", "S", "E", "Elist", "deflist", "def", "fn_dec", "fn_def", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // FooBisonParser::context.
  FooBisonParser::context::context (const FooBisonParser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  FooBisonParser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  FooBisonParser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  FooBisonParser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short FooBisonParser::yypact_ninf_ = -157;

  const signed char FooBisonParser::yytable_ninf_ = -1;

  const short
  FooBisonParser::yypact_[] =
  {
    -157,    13,  -157,    10,  -157,  -157,    19,   345,    -4,  -157,
      -9,    54,    58,   345,   345,   345,   345,   345,   529,    65,
     -53,    12,    49,  -157,  -157,  -157,  -157,  -157,  -157,   389,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,  -157,    33,  -157,    66,    69,
     -41,  -157,   665,    14,  -157,   665,   665,   665,   665,   665,
     711,   689,   782,   782,   109,   109,    38,    38,   665,   631,
     733,   755,   762,    38,    38,   -29,   -29,  -157,  -157,  -157,
     345,    73,   124,    23,    78,  -157,    66,   345,  -157,   345,
     665,    56,   -38,    31,    34,   328,   343,   101,    50,    57,
      46,  -157,  -157,  -157,  -157,   563,   104,  -157,    76,   182,
      32,   665,   323,   345,  -157,   345,    51,   107,  -157,   597,
      60,  -157,  -157,   345,   197,  -157,    83,   255,   345,  -157,
    -157,   665,   424,   328,   459,    61,  -157,  -157,    67,   665,
    -157,   345,  -157,   665,   270,   328,  -157,   328,   345,   345,
     665,  -157,   115,  -157,   494,    68,   328,    72,   345,  -157,
    -157,    62,   328,  -157
  };

  const signed char
  FooBisonParser::yydefact_[] =
  {
       2,     0,     1,     0,     4,     5,     0,     0,     0,    57,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    28,    27,    25,    26,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     3,     0,    67,     6,     0,
       0,    59,    61,     0,    56,    51,    52,    53,    54,    55,
      47,    48,    36,    37,    42,    43,    39,    41,    50,     0,
      46,    45,    44,    38,    40,    34,    35,    31,    32,    33,
       0,     0,     0,     0,     0,    68,     6,     0,    60,     0,
      63,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,    10,     6,    69,     7,     0,     0,     6,     0,     0,
       0,    62,    49,     0,    18,     0,     0,     0,    20,     0,
       0,    22,    23,     8,     0,    11,     0,     0,     0,    70,
       6,    65,     0,     0,     0,     0,    21,    19,     0,     9,
      12,     0,    71,    64,     0,     0,    16,     0,     0,     8,
      66,    72,    14,    15,     0,     0,     0,     0,     8,    13,
      17,     0,     0,    24
  };

  const short
  FooBisonParser::yypgoto_[] =
  {
    -157,  -157,   -36,  -156,   -94,    -7,  -157,  -157,    45,  -157,
    -157
  };

  const unsigned char
  FooBisonParser::yydefgoto_[] =
  {
       0,     1,    92,   148,   114,   115,    63,    21,    93,     4,
       5
  };

  const unsigned char
  FooBisonParser::yytable_[] =
  {
      18,    19,   124,   165,    57,    58,    25,    26,    27,    28,
      29,   127,   171,     2,     6,    62,    95,    96,     3,    52,
      53,    54,    22,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    59,   156,
      97,    22,     9,    10,     9,    10,     7,    20,    23,   116,
     119,   162,    24,   163,    11,    12,    11,    12,   116,    56,
      90,    91,   169,    60,    94,    98,   134,   101,   173,     8,
     117,   137,   118,   100,    50,    51,    52,    53,    54,   140,
     121,   125,   122,   123,   126,    13,    14,    13,    14,   129,
      15,    16,    15,    16,   154,   130,   133,   131,   136,    17,
      61,    17,   143,   138,   132,   145,   141,   147,   142,   144,
     151,   158,   166,   172,   159,   168,   149,     9,   102,   170,
     103,   153,   104,   105,   106,   107,   108,   109,   110,    11,
      12,   120,    41,    42,   160,     0,     0,     0,     0,     0,
       0,   164,   149,    48,    49,    50,    51,    52,    53,    54,
       0,   149,     0,     0,     0,     0,     0,     0,     0,     0,
      13,    14,     0,     0,     0,    15,    16,     0,     0,     0,
       0,   111,   112,   113,    17,     9,   102,     0,   103,     0,
     104,   105,   106,   107,   108,   109,   110,    11,    12,     0,
       9,   102,     0,   103,     0,   104,   105,   106,   107,   108,
     109,   110,    11,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,    14,
       0,     0,     0,    15,    16,     0,     0,     0,     0,   111,
     112,   139,    17,    13,    14,     0,     0,     0,    15,    16,
       0,     0,     0,     0,   111,   112,   150,    17,     9,   102,
       0,   103,     0,   104,   105,   106,   107,   108,   109,   110,
      11,    12,     0,     9,   102,     0,   103,     0,   104,   105,
     106,   107,   108,   109,   110,    11,    12,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    13,    14,     0,     0,     0,    15,    16,     0,     0,
       0,     0,   111,   112,   152,    17,    13,    14,     0,     0,
       0,    15,    16,     0,     0,     0,     0,   111,   112,   161,
      17,     9,   102,     0,   103,     0,   104,   105,   106,   107,
     108,   109,   110,    11,    12,     0,     9,    10,     9,    10,
      35,    36,    37,    38,    39,    40,    41,    42,    11,    12,
      11,    12,    44,     0,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    13,    14,     0,     0,     0,    15,
      16,     0,     0,     0,     0,   111,   112,     0,    17,    13,
      14,    13,    14,     0,    15,    16,    15,    16,     0,     0,
     128,     0,     0,    17,     0,    17,    30,    31,    32,    33,
      34,     0,     0,     0,     0,     0,    35,    36,    37,    38,
      39,    40,    41,    42,     0,     0,    43,     0,    44,     0,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
       0,    30,    31,    32,    33,    34,     0,     0,     0,     0,
      64,    35,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,     0,    44,     0,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,    30,    31,    32,    33,
      34,     0,     0,     0,     0,   155,    35,    36,    37,    38,
      39,    40,    41,    42,     0,     0,    43,     0,    44,     0,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
       0,    30,    31,    32,    33,    34,     0,     0,     0,     0,
     157,    35,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,     0,    44,     0,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,    30,    31,    32,    33,
      34,     0,     0,     0,     0,   167,    35,    36,    37,    38,
      39,    40,    41,    42,     0,     0,    43,     0,    44,     0,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      30,    31,    32,    33,    34,     0,    55,     0,     0,     0,
      35,    36,    37,    38,    39,    40,    41,    42,     0,     0,
      43,     0,    44,     0,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    30,    31,    32,    33,    34,     0,
     135,     0,     0,     0,    35,    36,    37,    38,    39,    40,
      41,    42,     0,     0,    43,     0,    44,     0,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    30,    31,
      32,    33,    34,     0,   146,     0,     0,     0,    35,    36,
      37,    38,    39,    40,    41,    42,     0,     0,    43,     0,
      44,    99,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    30,    31,    32,    33,    34,     0,     0,     0,
       0,     0,    35,    36,    37,    38,    39,    40,    41,    42,
       0,     0,    43,     0,    44,     0,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    35,     0,    37,    38,
      39,    40,    41,    42,     0,     0,     0,     0,     0,     0,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      37,    38,    39,    40,    41,    42,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    37,    38,    39,    40,    41,    42,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    37,    38,    39,    40,    41,    42,
       0,    37,    38,    39,    40,    41,    42,     0,    47,    48,
      49,    50,    51,    52,    53,    54,    48,    49,    50,    51,
      52,    53,    54,    39,    40,    41,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54
  };

  const short
  FooBisonParser::yycheck_[] =
  {
       7,     5,    40,   159,    57,    58,    13,    14,    15,    16,
      17,   105,   168,     0,     4,    22,    57,    58,     5,    48,
      49,    50,    60,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    36,   143,
      36,    60,     3,     4,     3,     4,    37,    61,     4,    36,
      96,   155,     4,   157,    15,    16,    15,    16,    36,     4,
      37,     5,   166,    61,     5,    61,   112,     4,   172,    60,
      57,   117,     4,    90,    46,    47,    48,    49,    50,    57,
      97,    60,    99,    37,    60,    46,    47,    46,    47,   106,
      51,    52,    51,    52,   140,     4,    60,    57,     4,    60,
      61,    60,    61,    37,    57,     8,   123,    57,   125,   126,
      37,    60,     7,    61,    57,    57,   133,     3,     4,    57,
       6,   138,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    96,    33,    34,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,    44,    45,    46,    47,    48,    49,    50,
      -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,     3,     4,    -1,     6,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    -1,
       3,     4,    -1,     6,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    -1,    -1,    57,
      58,    59,    60,    46,    47,    -1,    -1,    -1,    51,    52,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,     3,     4,
      -1,     6,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,     3,     4,    -1,     6,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    46,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,     3,     4,    -1,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,     3,     4,     3,     4,
      27,    28,    29,    30,    31,    32,    33,    34,    15,    16,
      15,    16,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    46,    47,    -1,    -1,    -1,    51,
      52,    -1,    -1,    -1,    -1,    57,    58,    -1,    60,    46,
      47,    46,    47,    -1,    51,    52,    51,    52,    -1,    -1,
      57,    -1,    -1,    60,    -1,    60,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      61,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    37,    -1,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    61,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      61,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    37,    -1,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    61,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    37,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      17,    18,    19,    20,    21,    -1,    57,    -1,    -1,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      37,    -1,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    17,    18,    19,    20,    21,    -1,
      57,    -1,    -1,    -1,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    37,    -1,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    17,    18,
      19,    20,    21,    -1,    57,    -1,    -1,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    37,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    37,    -1,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    27,    -1,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    29,    30,    31,    32,    33,    34,
      -1,    29,    30,    31,    32,    33,    34,    -1,    43,    44,
      45,    46,    47,    48,    49,    50,    44,    45,    46,    47,
      48,    49,    50,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50
  };

  const signed char
  FooBisonParser::yystos_[] =
  {
       0,    63,     0,     5,    71,    72,     4,    37,    60,     3,
       4,    15,    16,    46,    47,    51,    52,    60,    67,     5,
      61,    69,    60,     4,     4,    67,    67,    67,    67,    67,
      17,    18,    19,    20,    21,    27,    28,    29,    30,    31,
      32,    33,    34,    37,    39,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    57,     4,    57,    58,    36,
      61,    61,    67,    68,    61,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      37,     5,    64,    70,     5,    57,    58,    36,    61,    40,
      67,     4,     4,     6,     8,     9,    10,    11,    12,    13,
      14,    57,    58,    59,    66,    67,    36,    57,     4,    64,
      70,    67,    67,    37,    40,    60,    60,    66,    57,    67,
       4,    57,    57,    60,    64,    57,     4,    64,    37,    59,
      57,    67,    67,    61,    67,     8,    57,    57,    65,    67,
      59,    37,    59,    67,    64,    61,    66,    61,    60,    57,
      67,    59,    66,    66,    67,    65,     7,    61,    57,    66,
      57,    65,    61,    66
  };

  const signed char
  FooBisonParser::yyr1_[] =
  {
       0,    62,    63,    63,    63,    63,    64,    64,    65,    65,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    68,    68,    69,    69,    70,    70,    71,    71,    72,
      72,    72,    72
  };

  const signed char
  FooBisonParser::yyr2_[] =
  {
       0,     2,     0,     6,     2,     2,     0,     2,     0,     1,
       1,     2,     3,     7,     5,     5,     4,     7,     2,     3,
       2,     3,     2,     2,     9,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       4,     1,     3,     4,     6,     4,     5,     5,     6,     7,
       8,     9,    10
  };




#if YYDEBUG
  const short
  FooBisonParser::yyrline_[] =
  {
       0,   953,   953,   956,   967,   968,   971,   972,   975,   976,
     978,   979,   980,   981,   982,   983,   984,   985,   986,   987,
     988,   989,   990,   991,   992,  1005,  1006,  1007,  1008,  1009,
    1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,  1067,  1071,
    1075,  1081,  1082,  1094,  1108,  1124,  1134,  1148,  1153,  1160,
    1167,  1175,  1182
  };

  void
  FooBisonParser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  FooBisonParser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  FooBisonParser::symbol_kind_type
  FooBisonParser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,     2,     2,     2,    50,    43,     2,
      60,    61,    48,    46,    36,    47,     2,    49,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    40,    57,
      44,    37,    45,    39,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,    41,    59,    52,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    38,    53,    54,    55,    56
    };
    // Last valid token kind.
    const int code_max = 295;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 17 "fooParser.yy"
} // foo
#line 2725 "fooParser.tab.cc"

#line 1191 "fooParser.yy"


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
