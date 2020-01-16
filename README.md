# Compiler for Rat18F Programming Language

## Project Overview
This compiler compiles a custom programming language called, Rat18F. This was the final project for CPSC 323 \- Compilers and Languages, at California State University, Fullerton.

### Development Team
- Dion W. Pieterse
- Cong Le
- San Tran

### Create Executable &amp; Running Test Cases:
#### Create Executable
g\+\+ \-Wall \-std=c\+\+11 main\.cpp Lexer\.cpp Parser\.cpp \-o Exec

#### Run a Test Case via Console
./Exec test_case_#.txt

## Stage 1 of 3: Build a Lexical Analyzer
The Rat18F language rules and requirements were reviewed and all valid tokens were determined. We coded a lexical analyzer that tokenizes the input file in strict accordance to valid Rat18F tokens. Every token has valid instances (lexemes). The lexical analyzer writes to both the console and a text file listing each token, its pertaining lexeme, line number, and character position.

## Stage 2 of 3: Build a Syntax Analyzer
The second stage was to code a syntax analyzer. There are many approaches to making a parser. We coded a Top-Down Parser (Recursive Descent Parser).

The Rat18F syntax rules were thoroughly analyzed. Every rule had to be rewritten in order to eliminate left recursion. Left factorization also had to be used (if it was required). Finally, after the rewritten rules were double checked, we coded it out.

The process works as follows, the lexical analyzer is used to get the tokens. The Recursive Descent Parser will print each token, and pertaining lexeme found, and every production rule used for the analysis of each specific token, to both the console, and an output text file.

### On/Off Switch for Print Statements
An on/off "switch" for the print statement that prints each production rule was added as well.

### Error Handling
The parser generates helpful, and meaningful error messages which let the user know details such as:
1. The line number
2. The token
3. The lexeme
4. The type of error that occurred

## Stage 3 of 3: Symbol Table Handling &amp; Assembly Code Generation
The final stage of this compiler involved implementing Symbol Table Handling features, and assembly code generation. Additional code was written for the parser program to enable it to produce assembly code instructions. The target machine is a virtual machine. The virtual machine is based on a stack with specific types of assembly code instructions that needed to be implemented and coded. Every assembly instruction has a specific stack procedure (not detailed here), which had to be adhered to. We could only utilize the requested assembly instructions. Both the Symbol Table and Instruction Table are output to the console, and a text file.
