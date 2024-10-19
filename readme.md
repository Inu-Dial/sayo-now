# Readme

​	sayo-now is a C-style compiler for SayoDevice! asm script code written by @Yang. Run `sayo-now.exe --help` for option hints. You may translate the output asm code into json with my previous tool.

## Format

A sayo-now code is made up of three kinds of code blocks:

1. Global variable definitions
2. Function declaration
3. Function definition

Although written as `Type`, all `Type`s below mean `int` for there is no type system in either sayo-now or asm.

**Note that constants are SIGNED int, so please handle muliplications (neg) and overflow (undefined behaviour when happened in signed int)**

Global variable definitions each look like `Type Name = address/variable`.

​	note. You must define global variables and function arguments following `=address/variable` to assign address for variables manually, for there is no dynamic address access besides `Rx` and `P_Rx`.

A function declaration looks like 

```c++
Type func_name(int a=address/variable,int b=...);
```

and a function definition looks like

```c++
Type func_name(int a=address/variable,int b=...){ int x=address/variable, y=...; Sentences }
```

where `int x=...` is optional.

There must be a declaration/definition **before** called, and the number of defined arguments and their addresses (no need for names, however) must be **the same** as declaration's (if exists).

Before a function is called, all non-global variables in this environment will be pushed into system_stack to save their values, so be careful, **do not**  change their value in function_arguments, although you know what you are doing.

There must be a "main" definition. For global script arguments, use Vx (as shown in prog.hpp).

There are all operations supported by C expect for `(type)` (note again that there is no type system), `&` (reference) and `*` (dereference), although may not be efficient enough.

Number constants can be decimal, hexadecimal (e.g. 0x123abc), or binary (e.g. 0b10101).

### loops
`for`, `while` and `do ... while`
`continue` and `break`

## Built-in functions

`__addr __var` :

​	`__addr(x)`, which extracts the address of variable `x`, as an alternative for the loss of `operator &`, while `__var(x)` generates the variable with an address of  the constant `x`, similar to `operator *`.

`__raw` :

​	`__raw(c1,c2,...)` generates an asm command sentence `c1 c2 ...`, which allows you to make any asm code.

## About the example program (prog.cpp prog.hpp)

For sayo-now code is a valid C++ program with the header, so files are named with `cpp` and `hpp` to apply C++ highlight.

There are some useful macros in the header `prog.hpp` that you may use `g++` with `-E` option to apply to the program, and `-DSAYO_NOW` option should also be enabled for `g++` to avoid the valid-making macros, as shown in the header. This option adds a definition `SAYO_NOW`, such that g++ will ignore the contents between `#ifndef SAYO_NOW` and 1st `#endif`, but IDE will not.

In the makefile shows how to set `g++` and `sayo-now` options, pre-process with the header, and finally turn codes into asm. If you don't have a bash, you can add `.exe` suffix after `g++` and `sayo-now`, and run it in `cmd`. I have prepared `make.sh` for ` bash` (which Linux should be installed with), and `make.bat` for `cmd` (for Windows) for reference.

I have `g++` ready in this directory, but when there run out problems, you should go and get one `g++` installed properly.

`g++` pre-processed code goes along with the original line numbers, but `sayo-now` doesn't read it, however, for there can't be large projects for SayoDevice! ... maybe? So when `g++` reports warnings/errors, go to the original file, and for `sayo-now`, go to the  pre-processed file instead.

As you can see, there are two macros `get_array` and `set_array`. `get_array` can get the value at `Rx+i`，and `set_array` sets it.
The two macros are for later use, after `malloc` is implemented by Sayo.

## In the end

​	As a high school student, my program (and maybe the grammar in this `readme.md`? ) may not be strong enough. So when you find bugs / meet problems, take easy and report it to me. I'll provide my best help.