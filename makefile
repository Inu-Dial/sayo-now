# INC = -I "C:\Program Files\win_flex_bison" 
CC = g++ -O2 -std=c++23
INC = -I .
OUTPUT_DIR = dst

.RECIPEPREFIX = >
.PHONY: clean

# sayo-now/prog.out: sayo-now/sayo-now sayo-now/prog.in
# > sayo-now/sayo-now sayo-now/prog.in -o sayo-now/prog.out

# sayo-now/prog.in: sayo-now/prog.cpp
# > $(CC) $^ -o $@ -E -DSAYO_NOW

sayo-now/sayo-now: main.o fooParser.tab.o lex.yy_foo_.o FooLexer.o
> $(CC) $^ -o $@ $(INC)

main.o: main.cc FooLexer.hh FooLexer.cc fooParser.tab.hh location_t.hh
> $(CC) -c $< -o $@ $(INC)

lex.yy_foo_.o: lex.yy_foo_.cc FooLexer.hh fooParser.tab.hh location_t.hh
> $(CC) -c $< -o $@ $(INC)

fooParser.tab.o: fooParser.tab.cc FooLexer.hh location_t.hh
> $(CC) -c $< -o $@ $(INC)

FooLexer.o: FooLexer.cc
> $(CC) -c $< -o $@ $(INC)

lex.yy_foo_.cc: fooLexer.ll
> flex $<

fooParser.tab.hh fooParser.tab.cc fooParser.output: fooParser.yy
> bison $< -Wcounterexamples


clean:
> rm -rf prog main.o lex.* fooParser.tab.* fooParser.output FooLexer.o