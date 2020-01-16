/*******************************************************************************
 * Title:                   Compiler for Rat18F Programming Language           *
 * Course:                  CPSC 323 - Compilers and Languages                 *
 * Team:                    Cong Le, Dion W Pieterse, San Tran                 *
 * File:                    main.cpp                                           *
 ******************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include "Lexer.h"
#include "Parser.h"
using namespace std;

int main(int argc, char** argv) {
    //check for 2 args
    if(argc != 2){
        cerr <<  "Invalid number of arguments. Please try again." << endl;
        exit(EXIT_FAILURE);
    }
    else if(argc == 2 ){
        string test_file = argv[1];//convert cstr to string
        //Run Lexor() over the input file, and collect legal tokens into a list inside Lexer
        Lexer lex;
        lex.getFile(test_file);
        lex.printTokenPairsToConsole();
        lex.printTokenPairsToTxtFile();

        // Parsing list of token and its coresponding lexeme
        Parser par(lex.list,true,false);
        par.printSymbolOntoConsole();
        par.printInstructionOntoConsole();
        par.printSymbolIntoTxtFile();
        par.printInstructionIntoTxtFile();
    }
    return 0;
}
