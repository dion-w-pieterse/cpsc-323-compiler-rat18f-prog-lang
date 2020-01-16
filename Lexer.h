/*******************************************************************************
 * Title:                   Compiler for Rat18F Programming Language           *
 * Course:                  CPSC 323 - Compilers and Languages                 *
 * Team:                    Cong Le, Dion W Pieterse, San Tran                 *
 * File:                    Lexer.h                                            *
 ******************************************************************************/

#ifndef Lexer_hpp_
#define Lexer_hpp_

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

// Keep track of types of tokens and their lexemes
struct TokenLexPair {
    std::string token;
    std::string lexeme;
    int lineNum;
    int charNum;
};

class Lexer {
public:
    // Default constructor
    Lexer():lineNum(1),charNum(1),IDState_(0),Input_(nullptr),IntRealState_(0){ }
    // Destructor
    ~Lexer(){ Input_ = nullptr; delete Input_;  }
    void setStream(std::istream &stream){ Input_ = &stream; }
    // Vector to collect tokens and their lexemes from input file
    std::vector<TokenLexPair> getTokens();
    // Vector of tokens and their lexemes (final results)
    std::vector<TokenLexPair> list;

    void getFile(string &fileName);
    void printTokenPairsToTxtFile();
    void printTokenPairsToConsole();

protected:
    ifstream inputStream;  // input text file
    ofstream outputStream; // output text file
private:

    int lineNum;
    int charNum;
    unsigned int IDState_;
    std::istream *Input_;
    unsigned int IntRealState_;
    int isSeparator(const char &c);
    int isSeparator(const std::string &s);
    int isOperator(const char &c);
    int isOperator(const std::string &s);
    bool isKeyword(const std::string &s);
    bool isTerminator(const char &c);
    bool isspace_string(std::string s);
    void updateLinePosition(char c);
    TokenLexPair getPair();
    //=========================================================================
    // Legal tokens
    //=========================================================================
    // Defined keywords
    const std::unordered_set<std::string> KeywordSet_ = { "while",
        "whileend","return","if","ifend","true","false","int","boolean",
        "real","else","function","put","get" };

    // Defined Separators
    const std::unordered_set<std::string> SeparatorSet_ = {";","{",
        "}", "(",")", ",", "[", "]", ":", "$$"};

    // Defined Operators
    const std::unordered_set<std::string> OperatorSet_ = {"+","-","*",
        "/","=", "==", "^=", ">", "<", "=>", "=<"};
    //=========================================================================
    // 2 Finite State Machines(FSMs)
    const unsigned int INT_REAL_FSM_TABLE_[5][3] ={
        { 1,4,4 },
        { 1,2,4 },
        { 3,4,4 },
        { 3,4,4 },
        { 4,4,4 }};
    const unsigned int ID_FSM_TABLE_[4][3]= {
        {  1,3,3  },
        {  1,2,3  },
        {  1,2,3  },
        {  3,3,3  }};

    // Mutators for both FSMs
    void ID_FSM(const char &c);
    void INT_REAL_FSM(const char &c);

    // Helper functs for two FSMs
    void updateFSM(const char &c);
    void resetFSM();

    unsigned int Peek_INT_REAL_FSM(const char&c);
    unsigned int Peek_ID_FSM(const char&c);
};
#endif /* Lexer_hpp_ */
