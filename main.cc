#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include "FooLexer.hh"
#include "fooParser.tab.hh"
#include "help_info.hh"

extern const std::string help_info;

int main(int argc, char* argv[])
{
    if(argc == 0){
        puts("No input file detected, run with option --help for help info.");
        puts("compilation terminated.");
        return 0;
    }
    bool debug=false;
    bool sp_output=false;
    std::istream *in=&std::cin;
    std::ostream *out=&std::cout;
    FILE *fout=stdout;
    for(int i=1;i<argc;i++){
        std::string s(argv[i]);
        if(sp_output){
            // out=new std::ofstream(argv[i]);
            fout=fopen(&s[0],"w");
            // std::cerr<<"out \""<<argv[i]<<"\"\n";
            sp_output = false;
        }
        else if(s=="--debug"){
            debug = true;
        }
        else if(s.starts_with("-o")){
            if(s=="-o") sp_output=true;
            else fout=fopen(&s[2],"w");
        }
        else if(s=="--help"){
            puts(help_info.c_str());
            return 0;
        }
        else{
            in=new std::ifstream(s);
            // std::cerr<<"in \""<<argv[i]<<"\"\n";
        }
    }
    // const bool debug = argc > 1 && std::strcmp(argv[1], "--debug") == 0;
    std::stringstream sin(
        R"(
int __A=16;
int __B=17;
int a=123;
int b=456;
int c=789;
int func(int x=19);
void main(){
    a=1+2*2;
    b=a*a/a;
    c=func(b);
    d=(c>0);
    return 0;
}
int func(int x=19){
    return x*2;
}
        )"
    ),sout;
    foo::FooLexer lexer(*in,debug);
    foo::FooBisonParser parser(lexer, debug, sout);
    int parser_state = parser();
    fprintf(fout,"%s",&sout.str()[0]);
    return parser_state;
}