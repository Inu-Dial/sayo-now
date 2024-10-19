%{
    #include "FooLexer.hh"
    #include <string>
    #define YYSTYPE foo::bison_value_type
    #include "fooParser.tab.hh"
    
    using namespace foo;
    
    #undef  YY_DECL
    #define YY_DECL int FooLexer::yylex(bison_value_type *const lval, location_t *const lloc)
    
    #define YY_USER_INIT yylval = lval; yylloc = lloc;
    
    #define YY_USER_ACTION copyLocation(),copyValue();
%}

%option c++ noyywrap debug

%option yyclass="FooLexer"
%option prefix="yy_foo_"
%x WELL_COMMENT

%%

%{
    using Token = FooBisonParser::token;
%}


<WELL_COMMENT>\n { ++currentLine;currentColumn=1; BEGIN(INITIAL);}
<WELL_COMMENT>.+ {;}
\n { ++currentLine;currentColumn=1;}
#\ [0-9]+\ \"[^\"]+\"\ [0-9]+\n {
    ++currentLine;
    currentColumn=1;
    // currentLine=0;
    // yylval->str.pop_back();
    // int ten=1;
    // while(!yylval->str.empty()&&std::isdigit(yylval->str.back())){
    //     currentLine+=ten*(yylval->str.back()-'0');
    //     ten*=10;
    //     yylval->str.pop_back();
    // }
}
[[:space:]] ;
#  {BEGIN(WELL_COMMENT);}
if      {return Token::IF;}
else    {return Token::ELSE;}
while   {return Token::WHILE;}
do      {return Token::DO;}
return  {return Token::RETURN;}
continue {return Token::CONTINUE;}
break   {return Token::BREAK;}
for     {return Token::FOR;}
goto    {return Token::GOTO;}
void    {return Token::Type;}
int     {return Token::Type;}
[0-9]+       {return Token::Int_Constant;}
0x[0-9A-Fa-f]+    {return Token::Int_Constant;}
0b[0-1]+    {return Token::Int_Constant;}
[A-Za-z_][A-Za-z0-9_]* {return Token::Identifier; }
"++"    {return Token::ADD2;}
"--"    {return Token::SUB2;}
"+="    {return Token::Add_eq;}
"-="    {return Token::Sub_eq;}
"*="    {return Token::Mul_eq;}
"/="    {return Token::Div_eq;}
"%="    {return Token::Mod_eq;}
"<<="   {return Token::RL_eq;}
">>="   {return Token::RR_eq;}
"&="    {return Token::And_eq;}
"^="    {return Token::Xor_eq;}
"|="    {return Token::Or_eq;}
"&&"    {return Token::Logic_and;}
"||"    {return Token::Logic_or;}
"<<"    {return Token::RL;}
">>"    {return Token::RR;}
"=="    {return Token::EQ;}
"!="    {return Token::NE;}
"<="    {return Token::LE;}
">="    {return Token::GE;}
. { return yytext[0]; }

%%