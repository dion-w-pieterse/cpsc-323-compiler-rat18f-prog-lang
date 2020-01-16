/*******************************************************************************
 * Title:                   Compiler for Rat18F Programming Language           *
 * Course:                  CPSC 323 - Compilers and Languages                 *
 * Team:                    Cong Le, Dion W Pieterse, San Tran                 *
 * File:                    Parser.h                                           *
 ******************************************************************************/
#ifndef Parser_h
#define Parser_h
#include "Lexer.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// elements of each symbol
struct symbolElement{
    string id;
    int memoryLocation;
    string idType;
};
// elements of each instruction
struct instructionElement {
    int instructionStep;
    string op;
    int memoryAddress;
};

class Parser : public Lexer{
    typedef bool NON_FATAL_ERROR;
    typedef string Parser_Exception;
private:
    bool comment_switch;
    bool character_output;
    vector<TokenLexPair>::iterator iter;
    int instructionIndex;
    unsigned symbolIndex;

    // Table of symbols
    vector<symbolElement> symbolTable = vector<symbolElement>(30);
    // Table of instructions
    vector<instructionElement> instructionTable = vector<instructionElement>(1000);
    stack <int> jumpstack;
    string currentLexemeType;

public:
    Parser(){  // Default constructor
        comment_switch = true;
        symbolIndex = 0;
        instructionIndex = 1;      // instruction count starting from 1
        symbolTable[symbolIndex].memoryLocation = 0;
    }
    Parser(vector<TokenLexPair> &tok_lex_list, bool comments, bool char_output); // Parameterized constructor
    ~Parser() {};       // Destructor

    //Helper functions
    void print_tok_lexeme(std::vector<TokenLexPair>::iterator &iter);

    //toggle switch
    void inline flip_switch (bool adjust) { comment_switch = adjust; }

    // R1. <Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$
    void RAT(std::vector<TokenLexPair>::iterator &it);

    // R2. <Opt Function Definitions> ::= <Function Definitions> |  <Empty>
    void OFD(std::vector<TokenLexPair>::iterator &it);

    // R3. <Function Definitions>  ::= <Function> <Function Definition Prime>
    void FD(std::vector<TokenLexPair>::iterator &it);

    // R4. <Function Definition Prime> ::= <Function Definition> | epsilon
    void FD_Prime(std::vector<TokenLexPair>::iterator &it);

    // R5. <Function> ::= function <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>
    void F(std::vector<TokenLexPair>::iterator &it);

    // R6. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>
    void OPL(std::vector<TokenLexPair>::iterator &it);

    // R7. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>
    void PL(std::vector<TokenLexPair>::iterator &it);

    // R8. <Parameter> ::=  <IDs > : <Qualifier>
    void P(std::vector<TokenLexPair>::iterator &it);

    // R9. <Qualifier> ::= int     |    boolean    |  real
    void Q(std::vector<TokenLexPair>::iterator &it);

    // R10. <Body>  ::=  {  < Statement List>  }
    void B(std::vector<TokenLexPair>::iterator &it);

    // R11. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
    void ODL(std::vector<TokenLexPair>::iterator &it);

    // R12. <Declaration List>  ::= <Declaration> ; <Declaration List Prime>
    void DL(std::vector<TokenLexPair>::iterator &it);

    // R13. <Declaration List Prime> ::=     <Declaration List> | epsilon
    void DL_Prime(std::vector<TokenLexPair>::iterator &it);

    // R14. <Declaration> ::=   <Qualifier> <IDs>
    void D(std::vector<TokenLexPair>::iterator &it);

    // R15. <IDs> ::=     <Identifier> <IDs_Prime>
    void IDs(std::vector<TokenLexPair>::iterator &it);

    // R16. <IDs_Prime> ::=  ,<IDs> | epsilon
    void IDs_Prime(std::vector<TokenLexPair>::iterator &it);

    // R17. <Statement List> ::=   <Statement> <Statement List Prime>
    void SL(std::vector<TokenLexPair>::iterator &it);

    // R18. <Statement List Prime> ::=   <Statement List> | epsilon
    void SL_Prime(std::vector<TokenLexPair>::iterator &it);

    // R19. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>
    void S(std::vector<TokenLexPair>::iterator &it);

    // R20. <Compound> ::=   {  <Statement List>  }
    void COM(std::vector<TokenLexPair>::iterator &it);

    // R21. <Assign> ::=     <Identifier> = <Expression> ;
    void A(std::vector<TokenLexPair>::iterator &it);

    // R22. <If> ::=     if  ( <Condition>  ) <Statement> <If Prime>
    void IF(std::vector<TokenLexPair>::iterator &it);

    // R23. <If prime > ::=     ifend | else <Statement> ifend
    void IF_Prime(std::vector<TokenLexPair>::iterator &it);

    // R24. <Return> ::=  return <Return Prime>
    void RETURN(std::vector<TokenLexPair>::iterator &it);

    // R25. <Return Prime> ::=  ; | < Expression> ;
    void RETURN_Prime(std::vector<TokenLexPair>::iterator &it);

    // R26. <Print> ::=    put ( <Expression>);
    void PRINT(std::vector<TokenLexPair>::iterator &it);

    // R27. <Scan> ::=    get ( <IDs> );
    void SCAN(std::vector<TokenLexPair>::iterator &it);

    // R28. <While> ::=  while ( <Condition>  )  <Statement>  whileend
    void WHILE(std::vector<TokenLexPair>::iterator &it);

    // R29. <Condition> ::=     <Expression>  <Relop>   <Expression>
    void C(std::vector<TokenLexPair>::iterator &it);

    // R30. <Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<
    void R_op(std::vector<TokenLexPair>::iterator &it);

    // R31. <Expression>  ::=   <Term><Expression Prime>
    void E(std::vector<TokenLexPair>::iterator &it);

    // R32. <Expression Prime>  ::=   + <Term> <Expression Prime> | - <Term> <Expression Prime> | epsilon
    void E_Prime(std::vector<TokenLexPair>::iterator &it);

    // R33. <Term>    ::= <Factor> <Term Prime>
    void T(std::vector<TokenLexPair>::iterator &it);

    // R34. <Term Prime>    ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> | epsilon
    void T_Prime(std::vector<TokenLexPair>::iterator &it);

    // R35. <Factor> ::=      -  <Primary>    |    <Primary>
    void FAC(std::vector<TokenLexPair>::iterator &it);

    // R36. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false
    void PRIM(std::vector<TokenLexPair>::iterator &it);

    // R37. <Empty> ::= epsilon
    void EMPT(std::vector<TokenLexPair>::iterator &it);

    // Symbol Table Handling functions
    void genSymbol(std::vector<TokenLexPair>::iterator &it, string id_type);
    void printSymbolOntoConsole();
    void printSymbolIntoTxtFile();

    // Symbol Table Handling helper functions
    bool checkSymbol( string symbol);
    int getSymbolAddress(string saved) const;
    string getSymbolType(string input) const;
    void checkTypeMatch(string prevLexeme, string lexeme);

    // Functions Related to generating the Assembly Code
    void genInstruction(string op, int oprnd);
    void backPatch(int instr_address);
    void printInstructionOntoConsole();
    void printInstructionIntoTxtFile();
};
#endif /* Parser_h */
