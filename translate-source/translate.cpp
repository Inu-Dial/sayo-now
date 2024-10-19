#include<bits/stdc++.h>
#include<regex>
#include<ctime>
#include<sys/timeb.h>
using namespace std;
/*
 * Author: Yang
 * Date: 2024-02-09
 */
//#pragma GCC optimize(2)
typedef pair<int,int> pii;
typedef long long ll;
#define fst first
#define scd second
#define rep(i,s,t) for(int i=(s);i<=(t);++i)
#define irep(i,t,s) for(int i=(t);i>=(s);--i)
template<typename T>
bool chmax(T &x,T y){return y>x?(x=y,1):0;}
template<typename T>
bool chmin(T &x,T y){return y<x?(x=y,1):0;}
const int RULE_MAX = 4096;
vector<vector<string>> rules;
vector<int> sp;
vector<string> sp0,sp1,sp2;
vector<string> translate(string s,bool &stat){
    rep(i,0,(int)rules.size()-1){
        if(!regex_match(s,regex(rules[i][0])))continue;
        vector<string> r;
        rep(j,1,(int)rules[i].size()-1){
            r.push_back(regex_replace(s,regex(rules[i][0]),rules[i][j]));
        }
        stat=true;
        return r;
    }
    stat=false;
    return {s};
}
int read_num(string s){
    if(s.empty())return 0;
    if(s.size()==1)return s[0]-'0';
    if(s[0]=='0'&&(s[1]=='x'||s[1]=='X')){
        int x=0;
        rep(i,2,(int)s.size()-1){
            if('0'<=s[i]&&s[i]<='9')x=x*16+s[i]-'0';
            else if('a'<=s[i]&&s[i]<='f')x=x*16+10+s[i]-'a';
            else x=x*16+10+s[i]-'A';
        }
        return x;
    }
    int x=0;
    rep(i,0,(int)s.size()-1){
        if(s[i]<'0'||s[i]>'9')return -1;
        x=x*10+s[i]-'0';
    }
    return x;
}
bool is_hex(string s){
    return s.size()>1&&s[0]=='0'&&(s[1]=='x'||s[1]=='X');
}
bool is_num(string s){
    if(s.empty())return false;
    for(char c:s)if(c<'0'||c>'9')return false;
    if(s.size()>1&&s[0]==0)return false;
    return true;
}
vector<string> text,tmp;
void pb(string s){
    bool stat;
    auto vec=translate(s,stat);
    if(!stat){
        tmp.push_back(s);
        return;
    }
    for(string ss:vec)pb(ss);
}
string gt(){
    return "translate";
    time_t now=time(0);
    string s=ctime(&now);
    s.pop_back();
    return "translate: "+s;
}
void read_def(){
    FILE *def=fopen("translate_config/def.txt","r");
    vector<string> lst;
    string s="";
    char c=fgetc(def);
    while(true){
        if(c=='$'&&lst.empty()&&s.empty()){
            int x;
            char s0[50],s1[50],s2[50];
            fscanf(def,"%d %s %s %s",&x,s0,s1,s2);
            sp.push_back(x);
            sp0.push_back(s0);
            sp1.push_back(s1);
            sp2.push_back(s2);
        }
        else if(c==' '||c=='	'||c=='\n'||c=='\r'||c==EOF||c=='#'||c==';'){
            if(!s.empty())lst.push_back(s);
            s="";
            if(c==';')lst.push_back("");
            else if(c!=' '&&c!='	'){
                if(!lst.empty()){
                    rules.push_back(lst);
                    // cout<<lst.size()<<' ';
                    // for(auto s:lst)cout<<s<<' ';
                    // cout<<'\n';
                }
                lst.clear();
            }
        }
        else s+=c;
        if(c=='#'){
            while(c!='\n'&&c!='\r'&&c!=EOF)c=fgetc(def);
        }
        if(c==EOF)break;
        c=fgetc(def);
    }
}
map<int,int> ca;
void read_carg(FILE *log){
    FILE *f=fopen("translate_config/check.txt","r");
    char c=fgetc(f);
    string s;
    vector<string> cp;
    vector<vector<string>> vec;
    while(true){
        if(c==' '||c=='	'||c=='\n'||c=='\r'||c=='#'||c==EOF){
            if(!s.empty()){
                cp.push_back(s);
                s="";
            }
            if(c!=' '&&c!='	'){
                if(!cp.empty()){
                    for(auto s:cp)pb(s);
                    vec.push_back(tmp);
                    tmp.clear();
                    // cout<<lst.size()<<' ';
                    // for(auto s:lst)cout<<s<<' ';
                    // cout<<'\n';
                }
                cp.clear();
            }
            
        }
        else{
            s+=c;
        }
        if(c=='#'){
            while(c!='\n'&&c!='\r'&&c!=EOF)c=fgetc(f);
        }
        if(c==EOF)break;
        c=fgetc(f);
    }
    int lst=0;
    rep(i,0,(int)vec.size()-1){
        auto &v=vec[i];
        if(v.size()!=2&&v.size()!=1){
            fprintf(log,"[%s]warning: check.txt not correct on Line %d(begin with 0)!\n",&gt()[0],i);
        }
        for(auto &s:v){
            if(read_num(s)==-1){
                fprintf(log,"[%s]warning: %s is not a number(check.txt)\n",&gt()[0],&s[0]);
            }
        }
        if(v.size()==2)ca[read_num(v[0])]=lst=read_num(v[1]);
        else ca[read_num(v[0])]=lst;
    }
}
signed main(int _n, char * inp[]){
    // string a=R"(PUSH\((.+),(.+)\))";
    // cout<<regex_match("PUSH(1,2)",regex(a))<<'\n';
    // cout<<regex_replace("PUSH(1,2)",regex(a),"PUSH($2)")<<'\n';
    string script_name="out";
    if(_n >= 2){
        string path=inp[1];
        // cout<<_n<<'\n';
        // cout<<path<<'\n';
        string::size_type iPos = path.find_last_of('\\') + 1;
        string filename = path.substr(iPos, path.length() - iPos);
        // cout << filename << endl;

        //获取不带后缀的文件名
        script_name=filename.substr(0, filename.rfind("."));
        // cout << script_name << endl;

        freopen(inp[1],"r",stdin);
    }
    string script_name_suffix = script_name + ".json";
    // FILE *log=fopen("log.txt","w");
    FILE *log=stdout;
    FILE *fout=fopen(&script_name_suffix[0],"w");
    read_def();
    read_carg(log);


    char c=getchar();
    string s="";
    while(true){
        if(c==' '||c=='	'||c=='\n'||c=='\r'||c==EOF||c=='#'||c==';'){
            if(!s.empty())text.push_back(s);
            s="";
            if(c!=' '&&c!='	'&&c!=';'){
                if(!text.empty()&&!text.back().empty())
                    text.push_back("");
            }
            else if(c==';'){
                text.push_back(";");
            }
        }
        else{
            s+=c;
        }
        if(c=='#'){
            while(c!='\n'&&c!='\r'&&c!=EOF)c=getchar();
        }
        if(c==EOF)break;
        c=getchar();
    }
    if(text.empty()||text.back()!="")text.push_back("");

    // for(string s:text){
    //     if(s=="")cout<<'\n';
    //     else cout<<s<<" ";
    // }

    int p=0;
    while(p<text.size()){
        string s=text[p++];
        bool isp=false;
        rep(i,0,(int)sp.size()-1){
            if(s==sp0[i]){
                if(p==text.size()||text[p].empty()){
                    fprintf(log,"[%s]err: \"%s\" followed by nothing\n",&gt()[0],&s[0]);
                    return 1;
                }
                string num=text[p++];
                int x=read_num(num);
                if(x==-1){
                    fprintf(log,"[%s]err: \"%s\" is not a number\n",&gt()[0],&num[0]);
                    return 2;
                }
                isp=true;
                char n1[50],n2[50];
                sprintf(n1,"%d",x/sp[i]);
                sprintf(n2,"%d",x%sp[i]);
                if(x/sp[i]){
                    tmp.push_back(sp1[i]);
                    tmp.push_back(n1);
                }
                if(x/sp[i]&&x%sp[i])tmp.push_back("");
                if(x%sp[i]){
                    tmp.push_back(sp2[i]);
                    tmp.push_back(n2);
                }
                break;
            }
        }
        if(!isp){
            if(s=="def"){
                vector<string> rule;
                while(!text[p].empty()){
                    if(text[p]==";"){
                        rule.push_back("");
                        p++;
                        continue;
                    }
                    rule.push_back(text[p]);
                    p++;
                }
                rules.push_back(rule);
            }
            else
                tmp.push_back(s);
        }
    }
    text=tmp;
    tmp.clear();

    // for(string s:text){
    //     if(s=="")cout<<'\n';
    //     else cout<<s<<" ";
    // }

    for(string s:text)pb(s);
    text=tmp;
    tmp.clear();

    // for(string s:text){
    //     if(s=="")cout<<'\n';
    //     else cout<<s<<" ";
    // }
    

    set<string> labels;
    map<string,int> mp;
    vector<int> mrec;
    int prec=0;
    rep(i,0,(int)text.size()-1){
        string s=text[i];
        if(s=="label"){
            i++;
            if(i==text.size()||text[i].empty()){
                fprintf(log,"[%s]err: label followed by nothing\n",&gt()[0]);
                return 3;
            }
            labels.insert(text[i]);
        }
    }
    p=0;
    rep(i,0,(int)text.size()-1){
        string s=text[i];
        if(s=="label"){
            i++;
            mp[text[i]]=p;
        }
        else if(s.substr(0,4)=="rec_"){
            i++;i++;
            if(i>=text.size()||text[i-1].empty()||text[i].empty()){
                fprintf(log,"[%s]err: rec followed by nothing\n",&gt()[0]);
                return 4;
            }
            int x;
            sscanf(&s[4],"%d",&x);
            mrec.push_back(p+x);
            p+=6;
        }
        else if(labels.count(s))p+=2;
        else if(!text[i].empty())p++;
    }
    rep(i,0,(int)text.size()-1){
        string s=text[i];
        if(s=="label"){
            i++;
        }
        else if(s.substr(0,4)=="rec_"){
            i++;i++;
            char n[50]; 
            tmp.push_back("0x6f"); //MOVD
            tmp.push_back(text[i-1]);
            sprintf(n,"%d",mrec[prec]/256);
            tmp.push_back(n);
            tmp.push_back("");
            tmp.push_back("0x6f"); //MOVD
            tmp.push_back(text[i]);
            sprintf(n,"%d",mrec[prec]%256);
            tmp.push_back(n);
            // fprintf(log,"%d\n",mrec[prec]);
            prec++;
        }
        else if(labels.count(s)){
            int x=mp[s];
            char n1[50];
            sprintf(n1,"%d",x);
            tmp.push_back(n1);
        }
        else tmp.push_back(s);
    }
    text=tmp;
    tmp.clear();

    for(string &s:text){
        if(is_hex(s)){
            char n[50];
            sprintf(n,"%d",read_num(s));
            s=n;
        }
    }

    timeb t;
	ftime(&t);
    fprintf(fout,"{\"lastModify\":%lld,\"name\":\"%s\",\"steps\":[",t.time*1000+t.millitm,&script_name[0]);
    vector<string> args;
    p=0;
    bool fir=1;
    int sz=0;
    for(string s:text){
        if(s.empty()){
            if(args.empty())continue;
            if(!fir)fprintf(fout,",");
            fir=0;
            p++;
            if(args.size()==1){
                fprintf(fout,"{\"mode\":%s,\"values\":[]}",&args[0][0]);
            }
            else{
                fprintf(fout,"{\"mode\":%s,\"values\":[%s]}",&args[0][0],&args[1][0]);
            }
            if(!ca.count(read_num(args[0]))){
                stringstream ss;
                ss<<hex<<read_num(args[0]);
                string shex=ss.str();
                fprintf(log,"[%s]warning: opt %s(hex: %s) arg list size not set in check.txt!",&gt()[0],&args[0][0],&shex[0]);
            }
            else if(ca[read_num(args[0])]!=sz){
                stringstream ss;
                ss<<hex<<read_num(args[0]);
                string shex=ss.str();
                fprintf(log,"[%s]warning: opt %s(hex: %s) arg list size not match!\n",&gt()[0],&args[0][0],&shex[0]);
            }
            args.clear();
            sz=0;
        }
        else{
            if(args.size()<2){
                args.push_back(s);
            }
            else{
                args[1]+=",";
                args[1]+=s;
            }
            if(args.size()>=2)sz++;
            if(!is_num(s)){
                fprintf(log,"[%s]warning: \"%s\" is not a number\n",&gt()[0],&s[0]);
            }
        }
    }
    fprintf(fout,"]}");
    // system("pause");
}