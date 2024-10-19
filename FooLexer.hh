#pragma once

#include <string>
#include <random>
#include <time.h>
#include <sstream>
#include <algorithm>
#if ! defined(yyFlexLexerOnce)
#define yyFlexLexer yy_foo_FlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif
#include "location_t.hh"
using lexer_value_type = std::string;

namespace foo
{
    using std::string;
    // string get_rand_str();
    class Expr;
    enum class int_type{
        int32,
    }; 
    int_type get_int_type(string s);
    inline string to_str(unsigned int a){
        if(!a) return "0";
        string s;
        while(a) s+='0'+a%10,a/=10;
        std::reverse(s.begin(),s.end());
        return s;
    }
    inline string to_str(int a,int b){
        return to_str(a)+"_"+to_str(b);
    }
    using addr_type=unsigned int;
    using const_type=signed int;
    struct bison_value_type{
        string str;
        Expr* ast;
        std::vector<std::pair<string,addr_type>> defs;
    };
    const string fn_label_prefix = "__fn_";

    class FooLexer : public yy_foo_FlexLexer
    {
        std::size_t currentLine = 1, currentColumn = 1;
        
        bison_value_type *yylval = nullptr;
        location_t *yylloc = nullptr;
        
        void copyValue(const std::size_t leftTrim = 0, const std::size_t rightTrim = 0, const bool trimCr = false);
        void copyLocation() { 
            *yylloc = location_t(currentLine, currentColumn, currentLine, currentColumn + yyleng - 1); 
            currentColumn += yyleng;
        }
        
    public:
        FooLexer(std::istream &in, const bool debug) : yy_foo_FlexLexer(&in) { yy_foo_FlexLexer::set_debug(debug); }
        
        int yylex(bison_value_type *const lval, location_t *const lloc);
    };
    
    inline void FooLexer::copyValue(const std::size_t leftTrim, const std::size_t rightTrim, const bool trimCr)
    {
        std::size_t endPos = yyleng - rightTrim;
        if (trimCr && endPos != 0 && yytext[endPos - 1] == '\r')
            --endPos;
        yylval->str = std::string(yytext + leftTrim, yytext + endPos);
    }
}