/*******************************************************************************
 * Title:                   Compiler for Rat18F Programming Language           *
 * Course:                  CPSC 323 - Compilers and Languages                 *
 * Team:                    Cong Le, Dion W Pieterse, San Tran                 *
 * File:                    Parser.cpp                                         *
 ******************************************************************************/

#include "Parser.h"

#define NIL -9999
enum EXPRESSION_EXCEPTION { BOOL, INT, EPSILON  };
unsigned MEMORY_ADDRESS = 5000;
bool IS_FROM_SCAN = false;
bool IS_FROM_DECLARATION = false;
bool IS_FROM_PRINT =false;
string prevLexeme;
string tempSymbol;

// Parameterized constructor
Parser::Parser(vector<TokenLexPair> &tok_lex_list, bool comments, bool char_output) {
    //top down parser, start at RAT
    comment_switch = comments;          // turn on/off printing syntax rules
    character_output = char_output;
     instructionIndex = 1;              // instruction count starting from 1
    iter = tok_lex_list.begin();

    try{
        RAT(iter);
        cout << "Compilation Succesful\n";
        cout<<"Congratulation! Your syntax is 100% correct!\n";
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "Compilation Succesful\n";
            outputStream<<"Congratulations! Your syntax is 100% correct!\n";
        }
        outputStream.close();
    }
    catch(Parser_Exception &e) {
        cerr<<e<<endl;
        cerr << "Our program will exit due to encountering syntax error in source code.\n";
        // error message to txt file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<<e<<endl;
            outputStream << "Our program will exit due to encountering syntax error in source code.\n";
        }
        outputStream.close();
        exit(1);
    }
}

// print outputs to console and text file
void Parser::print_tok_lexeme(std::vector<TokenLexPair>::iterator &iter) {
    if(comment_switch){
        // print outputs to console
        cout << string(80, '*') << endl;
        cout << "*** FOUND *** Token: " << iter->token << " | " << "Lexeme: " << iter->lexeme << endl;
        cout << string(80, '*') << endl;

        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << string(80, '*') << endl;
            outputStream << "*** FOUND *** Token: " << iter->token << " | " << "Lexeme: " << iter->lexeme << endl;
            outputStream << string(80, '*') << endl;
        }
        outputStream.close();
    }
}

// R1. <Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$
void Parser::RAT(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
         // print production rule to console
        cout << "<Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$" << endl << endl;
        }
        outputStream.close();
    }
    OFD(it);
    if(it->lexeme == "$$") {
        ++it;
        print_tok_lexeme(it);
        ODL(it);
        SL(it);
        if(it->lexeme == "$$") {
            print_tok_lexeme(it);
            return;
        }
        else{
            // error msg
            Parser_Exception e= "Expecting a $$ at the end of main.\n";
            throw e;
        }
    }
    // error msg
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error, Expecting \"$$\" before \""+it->lexeme+"\"";
        throw e;
    }
}
// R2. <Opt Function Definitions> ::= <Function Definitions> |  <Empty>
void Parser::OFD(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl << endl;

       // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl << endl;
        }
        outputStream.close();
    }
    try{
        FD(it);
    }
    catch(NON_FATAL_ERROR &e) {
        //It was empty or we were at the end of the OPL
        return;
    }
}

// R3. <Function Definitions>  ::= <Function> <Function Definition Prime>
void Parser::FD(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Function Definitions>  ::= <Function> <Function Definition Prime>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Function Definitions>  ::= <Function> <Function Definition Prime>" << endl << endl;
        }
        outputStream.close();
    }
    F(it);
    FD_Prime(it);
}

// R4. <Function Definition Prime> ::= <Function Definition> | epsilon
void Parser::FD_Prime(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Function Definition Prime> ::= <Function Definition> | \u03B5" << endl << endl;
        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Function Definition Prime> ::= <Function Definition> | \u03B5" << endl << endl;
        }
        outputStream.close();
    }
    FD(it);
    // do nothing for epsilon case
}

// R5. <Function> ::= function <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>
void Parser::F(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Function> ::= function <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Function> ::= function <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>" << endl << endl;
        }
        outputStream.close();
    }
    print_tok_lexeme(it);
    if(it->lexeme == "function"){
        print_tok_lexeme(it);
        ++it;
        if(it->token == "Identifier") {
            print_tok_lexeme(it);
            ++it;
            if(it->lexeme == "("){
                print_tok_lexeme(it);
                ++it;
                OPL(it);
                if(it->lexeme ==")"){
                    print_tok_lexeme(it);
                    ++it;
                    ODL(it);
                    B(it);
                }
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"Fatal Error, Expecting \"(\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        else {
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"Fatal Error, \""+it->lexeme+"\" is not a valid identifier for function definition";
            throw e;
        }
    }
    else{
        //no longer function definition it should be $$, not a fatal error so we can go back up the stack
        NON_FATAL_ERROR e = true;
        throw e;
    }
}

// R6. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>
void Parser::OPL(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Opt Parameter List> ::=  <Parameter List> | <Empty>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Opt Parameter List> ::=  <Parameter List> | <Empty>" << endl << endl;
        }
        outputStream.close();
    }
    try{
        PL(it);
    }
    catch(NON_FATAL_ERROR &e){
        //It was empty or at the end of OPL
        return;
    }
}

// R7. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>
void Parser::PL(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Parameter List>  ::=  <Parameter> | <Parameter> , <Parameter List>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>" << endl << endl;
        }
        outputStream.close();
    }
    P(it);
    if(it->lexeme == ",") {
        print_tok_lexeme(it);
        ++it;
        PL(it);
        return;
    }
}

// R8. <Parameter> ::=  <IDs > : <Qualifier>
void Parser::P(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Parameter> ::=  <IDs > : <Qualifier>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Parameter> ::=  <IDs > : <Qualifier>" << endl << endl;
        }
        outputStream.close();
    }
    try{
        IDs(it);
    }
    catch(Parser_Exception &e) {
        NON_FATAL_ERROR f = true;
        throw f;
    }
    if(it->lexeme == ":") {
        print_tok_lexeme(it);
        ++it;
        Q(it);
    }
    else{
        --it;
        // error msg
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Parameter list: Fatal Error, Expecting \":\" before \""+it->lexeme+"\"";
        ++it;
        throw e;
    }
}

// R9. <Qualifier> ::= int     |    boolean    |  real
void Parser::Q(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Qualifier> ::= int     |    boolean    |  real" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Qualifier> ::= int     |    boolean    |  real" << endl << endl;
        }
        outputStream.close();
    }
    // current lexeme type is integer
    if(it->lexeme == "int"){
        currentLexemeType = it->lexeme;
        if(comment_switch){
            cout << "<Qualifier> -> integer\n";
        }
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // current lexeme type is boolean
    else if(it->lexeme == "boolean" ){
        currentLexemeType = it->lexeme;
        if(comment_switch){
            cout << "<Qualifier> -> boolean\n";
        }
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // current lexeme type is real
    else if(it->lexeme == "real"){
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // error msg
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error, \""+it->lexeme+"\" is not a valid qualifier";
        throw e;
    }
}


// R10. <Body>  ::=  {  < Statement List>  }
void Parser::B(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Body>  ::=  {  < Statement List>  }" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Body>  ::=  {  < Statement List>  }" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == "{") {
        print_tok_lexeme(it);
        ++it;
        SL(it);
        if(it->lexeme == "}") {
            print_tok_lexeme(it);
            ++it;
            return;
        }
        else{
            // error msg
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Body: Fatal Error, Expecting \"}\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    else{
        // error msg
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Body: Fatal Error, Expecting \"{\" before \""+it->lexeme+"\"";
        throw e;
    }
}

// R11. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
void Parser::ODL(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl << endl;
        }
        outputStream.close();
    }
    try{
        DL(it);
    }
    catch(NON_FATAL_ERROR &e) {
        //It was empty or at end of declaration list
        return;
    }
}

// R12. <Declaration List>  ::= <Declaration> ; <Declaration List Prime>
void Parser::DL(std::vector<TokenLexPair>::iterator &it){
    IS_FROM_DECLARATION = true;
    if(comment_switch) {
        // print production rule to console
        cout << "<Declaration List>  ::= <Declaration> ; <Declaration List Prime>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Declaration List>  ::= <Declaration> ; <Declaration List Prime>" << endl << endl;
        }
        outputStream.close();
    }
    D(it);
    if(it->lexeme == ";") {
        print_tok_lexeme(it);
        ++it;
        DL_Prime(it);
    }
    // error msg
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Declaration List: Fatal Error, Expecting \";\" before \"" +it->lexeme+ "\"";
        throw e;
    }
    IS_FROM_DECLARATION = false;
}

// R13. <Declaration List Prime> ::=     <Declaration List> | epsilon
void Parser::DL_Prime(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Declaration List Prime> ::=     <Declaration List> | \u03B5" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Declaration List Prime> ::=     <Declaration List> | \u03B5" << endl << endl;
        }
        outputStream.close();
    }
    DL(it);
    // epsilon case => do nothing.
}

// R14. <Declaration> ::=   <Qualifier> <IDs>
void Parser::D(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Declaration> ::=   <Qualifier> <IDs>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Declaration> ::=   <Qualifier> <IDs>" << endl << endl;
        }
        outputStream.close();
    }
    try{
        Q(it);
    }
    catch(Parser_Exception &e) {
        NON_FATAL_ERROR f = true;
        throw f;
    }
    IDs(it);
}

// R15. <IDs> ::=     <Identifier> <IDs_Prime>
void Parser::IDs(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<IDs> ::=     <Identifier> <IDs_Prime>" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<IDs> ::=     <Identifier> <IDs_Prime>" << endl << endl;
        }
        outputStream.close();
    }
    if(it->token == "Identifier") {
        print_tok_lexeme(it);
        //IS_FROM_DECLARATION
        //If not in table AND we are calling IDs from declaration production
        if(!checkSymbol(it->lexeme)&&IS_FROM_DECLARATION){
            // store new identifier with corresponding type, in table
            cout << "The identifer was not found in the symbol table, creating it..." << endl;
            genSymbol(it, currentLexemeType);
        }
        //else if IS in table AND calling IDs from scan production
        else if(checkSymbol(it->lexeme)&&IS_FROM_SCAN){
            genInstruction("STDIN", NIL);
            genInstruction("POPM",getSymbolAddress(it->lexeme));
        }
        //else if IS in table AND calling IDs from print production
        else if(checkSymbol(it->lexeme)&&IS_FROM_PRINT) {
            genInstruction("STDOUT", NIL);
        }
        // else if IS NOT in table
        else if(!checkSymbol(it->lexeme)){
            cerr << "Identifier " << it->lexeme << " has not been declared yet.\n";
            exit(1);
            //Alternatively, can throw parser exception here
        }
        ++it;
        try{
            IDs_Prime(it);
        }
        catch(EXPRESSION_EXCEPTION &e){
            if(e == EPSILON){
                // Do nothing
            }
        }
    }
    // error msg
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error,\""+it->lexeme +"\" is not a valid Identifier";
        throw e;
    }
}

// R16. <IDs_Prime> ::=  ,<IDs> | epsilon
void Parser::IDs_Prime(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<IDs_Prime> ::= ,<IDs> | \u03B5" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<IDs_Prime> ::= ,<IDs> | \u03B5" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == ",") {
        print_tok_lexeme(it);
        ++it;
        IDs(it);
    }
    // epsilon case
    else{
        EMPT(it);

    }
}

// R17. <Statement List> ::=   <Statement> <Statement List Prime>
void Parser::SL(std::vector<TokenLexPair>::iterator &it) {
    if(comment_switch) {
        // print production rule to console
        cout << "<Statement List> ::=   <Statement> <Statement List Prime>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Statement List> ::=   <Statement> <Statement List Prime>" << endl << endl;
        }
        outputStream.close();
    }
    S(it);
    try{
         SL_Prime(it);
    }
    catch( EXPRESSION_EXCEPTION &e){
        if(e == EPSILON){
            // Do nothing
        }
    }
}

// R18. <Statement List Prime> ::=   <Statement List> | epsilon
void Parser::SL_Prime(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Statement List Prime> ::=   <Statement List> | \u03B5" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Statement List Prime> ::=   <Statement List> | \u03B5" << endl << endl;
        }
        outputStream.close();
    }
    try{
        SL(it);
    }
    catch(NON_FATAL_ERROR &e){
        // do nothing for nonfatal error
        EMPT(it);    // epsilon case
    }
}

// R19. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>
void Parser::S(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>" << endl << endl;
        }
        outputStream.close();
    }
    try{
        COM(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //Do nothing
    }
    try{
        A(it);
        return;
    }
    catch(NON_FATAL_ERROR&e){
        //Do nothing
    }
    try{
        IF(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //Do nothing
    }
    try{
        RETURN(it);
        return;
    }
    catch(NON_FATAL_ERROR&e){
        //Do nothing
    }
    try{
        PRINT(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //Do nothing
    }
    try{
        SCAN(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //Do nothing
    }
    try{
        WHILE(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //Do nothing
    }

    if(it->lexeme == "$$" || it-> lexeme == "}"||it->lexeme == "") {
        NON_FATAL_ERROR e = true;
        throw e;
    }
    // error msg
    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)+":"+"Fatal Error, Statement beginning with "+it->lexeme +" not recognized";
    throw e;
}

// R20. <Compound> ::=   {  <Statement List>  }
void Parser::COM(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Compound> ::=   {  <Statement List>  }" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Compound> ::=   {  <Statement List>  }" << endl << endl;
        }
        outputStream.close();
    }

    if(it->lexeme == "{"){
        print_tok_lexeme(it);
        ++it;
        SL(it);
        if(it->lexeme == "}"){
            print_tok_lexeme(it);
            ++it;
            return;
        }
        // error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Compound: Fatal Error, Expecting \"}\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error msg
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R21. <Assign> ::=     <Identifier> = <Expression> ;
void Parser::A(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Assign> ::=     <Identifier> = <Expression> ;" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Assign> ::=     <Identifier> = <Expression> ;" << endl << endl;
        }
        outputStream.close();
    }
    if(it->token == "Identifier") {
        // check if its lexeme is in Symbol table
        if(!checkSymbol(it->lexeme) ){
            cerr << "Identifier " << it->lexeme << " has not been declared yet.\n";
            exit(1);
        }
        // save type of the symbol
        string symbolTypeSaved = getSymbolType(it->lexeme);
        // save symbol for address generation
        string symbolSaved = it->lexeme;
        // assign temp to symbol for future comparision
        tempSymbol = it->lexeme;

        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "="){
            print_tok_lexeme(it);
            ++it;
            try{
                E(it);
            }
            catch(EXPRESSION_EXCEPTION &e){
                cout << (e==INT?"<Expression> throw back an int":"<Expression> throw back a boolean") <<endl;
                // checking type compatibility of 2 symbols
                if((e == BOOL && symbolTypeSaved == "int")||(e == INT && symbolTypeSaved == "boolean")){
                    cerr << "Type mismatch in <Assign>\n";
                    exit(1);
                }
            }
            // get memory address of identifier symbol
            int address = getSymbolAddress(symbolSaved);
            // generating instruction POPM for symbol
            genInstruction("POPM", address);

            if(it->lexeme == ";"){
                print_tok_lexeme(it);
                ++it;
                return;
            }
            // error msg
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Assignment: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Assignment: Fatal Error, Expecting \"=\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    //non-fatal error
    else {
        NON_FATAL_ERROR e = true;
        throw e;
    }
}

// R22. <If> ::=     if  ( <Condition>  ) <Statement> <If Prime>
void Parser::IF(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<If> ::=     if  ( <Condition>  ) <Statement> <If Prime>" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<If> ::=     if  ( <Condition>  ) <Statement> <If Prime>" << endl << endl;
        }
        outputStream.close();
    }

    if(it->lexeme == "if"){
        //int address = instructionIndex;
        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "("){
            print_tok_lexeme(it);
            ++it;
            C(it);
            if(it->lexeme == ")"){
                print_tok_lexeme(it);
                ++it;
                S(it);
                IF_Prime(it);
            }
            // error msg
            else{
                --it;
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In IF-statement: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error msg
        else{
            --it;
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"in IF-statement: Fatal Error, Expecting \"(\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R23. <If prime > ::=     ifend | else <Statement> ifend
void Parser::IF_Prime(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<If prime > ::=     ifend | else <Statement> ifend" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<If prime > ::=     ifend | else <Statement> ifend" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == "ifend"){
        backPatch(instructionIndex);
        // LABEL for ifend
        genInstruction("LABEL",NIL);
        print_tok_lexeme(it);
        ++it;
    }
    else if( it->lexeme == "else"){
        genInstruction("JUMP", NIL);
        backPatch(instructionIndex);// this will back patch to line 10's Jumpz
        jumpstack.push(instructionIndex-1);
        // LABEL for else
        genInstruction("LABEL", NIL);

        print_tok_lexeme(it);
        ++it;
        S(it);
        backPatch(instructionIndex);
        genInstruction("LABEL", NIL);
        if(it->lexeme == "ifend"){
            print_tok_lexeme(it);
            it++;
        }
        //error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In IF-statement: Fatal Error, Expecting \"ifend\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error msg
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In IF-statement: Fatal Error, Expecting \"ifend\" or \"else\" before \""+it->lexeme+"\"";
        throw e;
    }
}

// R24. <Return> ::=  return <Return Prime>
void Parser::RETURN(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        cout << "<Return> ::=  return <Return Prime>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Return> ::=  return <Return Prime>" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == "return"){
        print_tok_lexeme(it);
        ++it;
        RETURN_Prime(it);
    }
    // non-fatal error
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R25. <Return Prime> ::=  ; | < Expression> ;
void Parser::RETURN_Prime(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Return Prime> ::=  ; | < Expression> ;" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Return Prime> ::=  ; | < Expression> ;" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == ";"){
        print_tok_lexeme(it);
        ++it;
        return;
    }
    else{
        E(it);
        if(it->lexeme == ";"){
            print_tok_lexeme(it);
            ++it;
            return;
        }
        // error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Return: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
}

// R26. <Print> ::=    put ( <Expression>);
void Parser::PRINT(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Print> ::=    put ( <Expression>);" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Print> ::=    put ( <Expression>);" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == "put"){
        IS_FROM_PRINT = true;

        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "("){
            print_tok_lexeme(it);
            ++it;
            try{
                E(it);
            }
            catch( EXPRESSION_EXCEPTION &e){
                // Do nothing here
            }
            if(it->lexeme == ")"){
                // generating instruction STDOUT
                genInstruction("STDOUT", NIL);
                print_tok_lexeme(it);
                ++it;
                if(it->lexeme == ";"){
                    print_tok_lexeme(it);
                    ++it;
                    IS_FROM_PRINT = false;
                    return;
                }
                // error msg
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"In Print: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            // error msg
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Print: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Print: Fatal Error, Expecting \"()\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // non-fatal error
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }

}

// R27. <Scan> ::=    get ( <IDs> );
void Parser::SCAN(std::vector<TokenLexPair>::iterator &it) {
    IS_FROM_SCAN = true;
    if(comment_switch) {
        // print production rule to console
        cout << "<Scan> ::=    get ( <IDs> );" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Scan> ::=    get ( <IDs> );" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == "get"){
        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "("){
            print_tok_lexeme(it);
            ++it;
            IDs(it);
            if(it->lexeme == ")"){
                print_tok_lexeme(it);

                ++it;
                if(it->lexeme == ";"){
                    print_tok_lexeme(it);

                    ++it;
                    return;
                }
                // error msg
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"In Scan: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            // error msg
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Scan: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Scan: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R28. <While> ::=  while ( <Condition>  )  <Statement>  whileend
void Parser::WHILE(std::vector<TokenLexPair>::iterator &it) {
    if(comment_switch) {
        // print production rule to console
        cout << "<While> ::=  while ( <Condition>  )  <Statement>  whileend" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<While> ::=  while ( <Condition>  )  <Statement>  whileend" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme == "while") {
        int address = instructionIndex;
        genInstruction("LABEL", NIL);  // generating instruction LABEL

        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "(") {
            print_tok_lexeme(it);
            ++it;
            C(it);

            if(it->lexeme == ")") {
                print_tok_lexeme(it);
                ++it;
                S(it);

                genInstruction("JUMP", address); // generating instruction JUMP
                genInstruction("LABEL", NIL); // This adds the label to end of while loop.
                backPatch(instructionIndex-1);


                if(it->lexeme == "whileend") {
                    print_tok_lexeme(it);
                    ++it;
                    return;
                }
                // error msg
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"In While: Fatal Error, Expecting \"whileend\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            // error msg
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In While: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In While: Fatal Error, Expecting \"(\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error msg
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R29. <Condition> ::=     <Expression>  <Relop>   <Expression>
void Parser::C(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Condition> ::=     <Expression>  <Relop>   <Expression>" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Condition> ::=     <Expression>  <Relop>   <Expression>" << endl << endl;
        }
        outputStream.close();
    }
    bool expression1Bool = false, expression2Bool = false;
    // check 1st <Expression>
    try{
        E(it);
    }
    catch (EXPRESSION_EXCEPTION &e){
        e = INT;
        expression1Bool = true;
    }
     // current relational operator
    string currentOperator = it->lexeme;
    R_op(it);

     // check 2nd <Expression>
    try{
        E(it);
    }
   catch(EXPRESSION_EXCEPTION &e){
        e = INT;
       expression2Bool = true;
    }
    // check type compatibility between 2 <Expression>,
    // both <Expression> MUST BE 2 int for arithmetic operations
    if((!expression1Bool && expression2Bool)||(expression1Bool&&!expression2Bool)){
        cerr << "Type mismatch in comparison\n";
        exit(1);
    }
    // Relational operator cases
    if(currentOperator == "=="){
        genInstruction("EQU", NIL);        // generating instruction EQU
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating instruction JUMPZ
    }
    else if(currentOperator == "^=" ){
        genInstruction("NEQ", NIL);        // generating instruction NEQ
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating instruction JUMPZ
    }
    else if(currentOperator == "=>" ){
        genInstruction("GEQ", NIL);        // generating instruction GEQ
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating instruction JUMPZ
    }
    else if(currentOperator == "=<" ){
        genInstruction("LEQ", NIL);        // generating instruction LEQ
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating instruction JUMPZ
    }
    else if(currentOperator == "<" ){
        genInstruction("LES", NIL);        // generating instruction LES
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating instruction JUMPZ
    }
    else if(currentOperator == ">" ){
        genInstruction("GRT", NIL);        // generating instruction GRT
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating instruction JUMPZ
    }
    // error msg
    else{
        cerr << "Relop syntax error at line "<<to_string(it->lineNum) +":"+to_string(it->charNum)<<endl;
        cerr << "Invalid operator\n"<< "'=', '/=', '>', '<', '=>' or '<=' is expected.\n";
        exit(1);
    }
}

// R30. <Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<
void Parser::R_op(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<" << endl << endl;
        }
        outputStream.close();
    }
    if(it->lexeme != "=="&&
       it->lexeme != "^="&&
       it->lexeme != ">"&&
       it->lexeme != "<"&&
       it->lexeme != "=>"&&
       it->lexeme != "=<") {
        --it;
        // error msg
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error, Unrecognized relational operator \"" + it->lexeme+"\"";
        throw e;
    }
    ++it;
}

// R31. <Expression>  ::=   <Term><Expression Prime>
void Parser::E(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Expression>  ::=   <Term><Expression Prime>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Expression>  ::=   <Term><Expression Prime>" << endl << endl;
        }
        outputStream.close();
    }

    bool T_Is_BOOL = false;
    try{
       T(it);
    }
    catch (EXPRESSION_EXCEPTION & e){
        if(e == BOOL){
            cout << "we confirm that test is a boolean\n";
            T_Is_BOOL = true;
        }
    }
    try{
        E_Prime(it);
    }
    catch(EXPRESSION_EXCEPTION & e){
        // <Expression Prime> returns EPSILON and <Term> returns BOOL value
        if(e == EPSILON && T_Is_BOOL){
            throw BOOL;
            return;
        }
        // <Expression Prime> returns EPSILON or INT and <Term> does not return BOOL value
        else if((e == EPSILON || e == INT)   && !T_Is_BOOL  ){
            throw INT;
            return;
        }
        // error msg
        else{
            cerr<<"We have a mismatched pair of variable type"<<endl;
            exit(1);
        }
    }
}

// R32. <Expression Prime>  ::=   + <Term> <Expression Prime> | - <Term> <Expression Prime> | epsilon
void Parser::E_Prime(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Expression Prime>  ::=   + <Term> <Expression Prime> | - <Term> <Expression Prime> |  \u03B5" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Expression Prime>  ::=   + <Term> <Expression Prime> | - <Term> <Expression Prime> |  \u03B5" << endl << endl;
        }
        outputStream.close();
    }
    // + <Term> <Expression Prime>
    if(it->lexeme == "+"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        ++it;
        T(it);
        // generating instruction ADD
        genInstruction("ADD", NIL);

        E_Prime(it);
    }
    // - <Term> <Expression Prime>
    else if(it->lexeme == "-"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        ++it;
        T(it);
         // generating instruction SUB
        genInstruction("SUB", NIL);

        E_Prime(it);
    }
    // epsilon case
    else {
        EMPT(it);
    }
}

// R33. <Term>    ::= <Factor> <Term Prime>
void Parser::T(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Term>    ::= <Factor> <Term Prime>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Term>    ::= <Factor> <Term Prime>" << endl << endl;
        }
        outputStream.close();
    }
    FAC(it);
    try{
        T_Prime(it);
    }
    catch(EXPRESSION_EXCEPTION &e  ){
        if((e == EPSILON)|| e == INT  ) {
            return;
        }
        else{
            throw e;
        }
    }
}

// R34. <Term Prime>    ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> | epsilon
void Parser::T_Prime(std::vector<TokenLexPair>::iterator &it) {
    if(comment_switch) {
        // print production rule to console
        cout << "<Term Prime>    ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> |  \u03B5" << endl << endl;

        // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Term Prime>    ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> |  \u03B5" << endl << endl;
        }
        outputStream.close();
    }

    // * <Factor> <Term Prime>
    if(it->lexeme == "*"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        it++;
        FAC(it);
        // generating the instruction MUL
        genInstruction("MUL", NIL);

        T_Prime(it);
    }
    // / <Factor> <Term Prime>
    else if(it->lexeme == "/"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        it++;
        FAC(it);
        // generating the instruction DIV
        genInstruction("DIV", NIL);

        T_Prime(it);
    }
    // epsilon case
    else{
        EMPT(it);
    }
}

// R35. <Factor> ::=      -  <Primary>    |    <Primary>
void Parser::FAC(std::vector<TokenLexPair>::iterator &it) {
    if(comment_switch) {
        // print production rule to console
        cout << "<Factor> ::=      -  <Primary>    |    <Primary>" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Factor> ::=      -  <Primary>    |    <Primary>" << endl << endl;
        }
        outputStream.close();
    }
     // - <Primary>
    if(it->lexeme == "-"){
        print_tok_lexeme(it);
        prevLexeme = it->lexeme;        // for futute comparision
        ++it;
        PRIM(it);
        if(tempSymbol == "boolean") {
            cerr << "Error- cannot have a negative boolean\n";
            exit(1);
        }
    }
    // <Primary>
    else{
        PRIM(it);
    }
}


// R36. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false
void Parser::PRIM(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false" << endl << endl;
        }
        outputStream.close();
    }

    // <Identifier>
    // INSTRUCTION GENERATION for Identifier case
    if(it->token== "Identifier") {
        // check if symbol is in table
        if(!checkSymbol(it->lexeme)){
            cerr << "Identifier " << it->lexeme << " has not been declared yet.\n";
            exit(1);
        }
         //  getting memory address of symbol
        int address = getSymbolAddress(it->lexeme);
        // generating instruction PUSHM
        genInstruction("PUSHM", address);
        // determine type of symbol
        string type_of_id = getSymbolType(it->lexeme);
        // if symbol is boolean
        if(type_of_id == "boolean"){
            ++it;
            throw BOOL;
        }
         // else the symbol is int
        else if(type_of_id == "int"){
            ++it;
            return;
        }
        print_tok_lexeme(it);
        ++it;

        // <Identifier>  ( <IDs> )
        if(it->lexeme == "(") {
            print_tok_lexeme(it);
            ++it;
            IDs(it);
            ++it;
            if(it->lexeme == ")"){
                print_tok_lexeme(it);
                return;
            }
            // error msg
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Primary: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
    }
     // <Integer>
    // Instruction generation for Integer case
    else if(it->token == "Int"){
        // some conversions
        int inputedInt = 0;

        // convert inputed string into negative int value
        if(prevLexeme == "-"){
            inputedInt = 0 - stoi(it->lexeme);
        }
        // convert inputed string into positive int value
        else{
            inputedInt = stoi(it->lexeme);
        }
        // PUSH int value on top of the stack
        genInstruction("PUSHI", inputedInt);
        tempSymbol = "Integer";
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // <Real>
    // No instruction generated in this case
    else if(it->token == "Real"){
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // true | false
    // Instruction generation for boolean cases
    else if(it->lexeme == "true" || it->lexeme == "false"){
        // determine if the the 2 variables having the same type and value
        if( (!(getSymbolType(tempSymbol) == "boolean")  && !(tempSymbol == "")) || prevLexeme == "-" ){
            // comparing a negative int and boolen value
            if (prevLexeme == "-"){
                cerr << "Cannot assign " << tempSymbol << " to " << prevLexeme + it->lexeme << endl;
            }
            // comparing a positive int and boolean value
            else {
                cerr << "The type of " << tempSymbol << " and " << it->lexeme << " must match" << endl;
            }
            exit(1);
        }
        // Valid boolean values
        // boolean as true value
        if(it->lexeme == "true"){
            // generating instruction PUSHI for true value as 1
            genInstruction("PUSHI", 1);
            print_tok_lexeme(it);
            ++it;
            throw BOOL;
        }
        // boolean as false value
        else {
            // generating instruction PUSHI for false value as 0
            genInstruction("PUSHI", 0);
            print_tok_lexeme(it);
            ++it;
            throw BOOL;
        }
    }
    // ( <Expression> )
    else if(it->lexeme == "(") {
        print_tok_lexeme(it);
        ++it;
        E(it);
        if(it->lexeme == ")"){
            print_tok_lexeme(it);
            ++it;
            return;
        }
        // error msg
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Primary: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error msg
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Primary: Fatal Error, "+it->lexeme+" is an unrecognized primary";
        throw e;
    }
}

// R37. <Empty> ::= epsilon
void Parser::EMPT(std::vector<TokenLexPair>::iterator &it){
    if(comment_switch) {
        // print production rule to console
        cout << "<Empty> ::=  \u03B5" << endl << endl;

         // print production rule to text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Empty> ::=  \u03B5" << endl << endl;
        }
        outputStream.close();
    }
    throw EPSILON;
}
//=================================================================================
// Functions used for Semantic rules
//=================================================================================
// Symbol Table Handling functions

// determine if lexeme already existing symbol in table
bool Parser::checkSymbol(string symbol){
    for(size_t i=0; i< symbolIndex; i++){
        if(symbol ==  symbolTable[i].id){
            return true;
        }
    }
    return false;
}
// generating Symbol Table and its elements with corresponsing values
void Parser::genSymbol(std::vector<TokenLexPair>::iterator &it, string id_type){
    symbolTable[symbolIndex].id = it->lexeme;
    symbolTable[symbolIndex].memoryLocation = MEMORY_ADDRESS;
    symbolTable[symbolIndex].idType = id_type;
    symbolIndex++;
    MEMORY_ADDRESS++;

}
// Print contents of Symbol Table to console
void Parser::printSymbolOntoConsole( ){
    cout<< "\nSYMBOL TABLE\n";
    cout<<setw(15)<<left<<"Identifier"<<setw(20)<<"Memory Location"<<setw(15)<<"Type"<<setw(20)<<left<<endl;
    for(size_t i= 0; i<symbolIndex; i++){
        cout<<setw(20)<<symbolTable[i].id <<setw(15) << symbolTable[i].memoryLocation <<setw(10) << symbolTable[i].idType <<endl;
    }
}
// Print contents of Symbol Table to text file
void Parser::printSymbolIntoTxtFile(){
    // open text file
    outputStream.open("Symbol_And_Instruction_Tables.txt");
    if(outputStream.is_open()){
         outputStream<< "Output:" <<endl;
        outputStream<< "\nSYMBOL TABLE\n";
        outputStream<<setw(15)<<left<<"Identifier"<<setw(20)<<"Memory Location"<<setw(15)<<"Type"<<setw(20)<<left<<endl;
        for(size_t i= 0; i<symbolIndex; i++){
            outputStream<<setw(20)<<symbolTable[i].id <<setw(15) << symbolTable[i].memoryLocation <<setw(10) << symbolTable[i].idType <<endl;
        }
    }
    // close text file
    outputStream.close();
}

// access memory address of saved variable
int Parser::getSymbolAddress(string saved) const{
    int address = 0;
    for(size_t i = 0; i < symbolIndex; i++){
        if(symbolTable[i].id == saved ){
            address = symbolTable[i].memoryLocation;
        }
    }
    return address;
}
// access type of already declared items in table
string Parser::getSymbolType(string input) const {
    string a = "";
    for(size_t i = 0; i< symbolIndex; i++){
        if(symbolTable[i].id == input){
            a = symbolTable[i].idType;
        }
    }
    if(input == "true" || input == "false" ){
        a = "boolean";
    }
    return a;
}
// no arithmetic operation for boolean values
void Parser::checkTypeMatch(string prevLexeme, string lexeme ){
    if(getSymbolType(prevLexeme) == "boolean" || getSymbolType(lexeme) == "boolean" ){
        cerr<< "No arithmetic operation are allowed for boolean."<<endl;
        exit(1);
    }
}
//=================================================================================
// Functions Related to generating the Assembly Code

void Parser::genInstruction(string op, int oprnd){
    instructionTable[instructionIndex].instructionStep = instructionIndex;
    instructionTable[instructionIndex].op = op;
    instructionTable[instructionIndex].memoryAddress = oprnd;
    instructionIndex++;
}
// save the address memory for JUMPZ
void Parser::backPatch(int jump_addr){
    int address = jumpstack.top();
    instructionTable[address].memoryAddress = jump_addr;
    jumpstack.pop();
}
// Print contents of Instruction Table to console
void Parser::printInstructionOntoConsole( ){
    cout<< "\nINSTRUCTION TABLE\n";
    string nil;
    cout<<setw(10)<<left<<"Address"<<setw(10)<<"Operator"<<setw(7)<<"Operand"<<setw(20)<<left<<endl;
    for(int i = 1; i< instructionIndex; i++){
        if(instructionTable[i].memoryAddress == NIL ){
            nil = "";
            cout<<setw(12)<<left<<instructionTable[i].instructionStep<< setw(10)<< instructionTable[i].op <<nil<<endl;
        }else{
            cout<<setw(12)<<left<<instructionTable[i].instructionStep<<setw(10)<< instructionTable[i].op <<instructionTable[i].memoryAddress<<endl;
        }
    }
}
// Print contents of Instruction Table to text file
void Parser::printInstructionIntoTxtFile(){
    // open text file
    outputStream.open("Symbol_And_Instruction_Tables.txt",std::ofstream::app);

    if(outputStream.is_open()){
        outputStream<< "\nINSTRUCTION TABLE\n";
        string nil;
        outputStream<<setw(10)<<left<<"Address"<<setw(10)<<"Operator"<<setw(7)<<"Operand"<<setw(20)<<left<<endl;
        for(int i = 1; i< instructionIndex; i++){
            if(instructionTable[i].memoryAddress == NIL ){
                nil = "";
                outputStream<<setw(12)<<left<<instructionTable[i].instructionStep<< setw(10)<< instructionTable[i].op <<nil<<endl;
            }else{
                outputStream<<setw(12)<<left<<instructionTable[i].instructionStep<<setw(10)<< instructionTable[i].op <<instructionTable[i].memoryAddress<<endl;
            }
        }
    }
    // close text file
    outputStream.close();
}
