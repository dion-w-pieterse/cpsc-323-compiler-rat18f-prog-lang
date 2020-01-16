/*******************************************************************************
 * Title:                   Compiler for Rat18F Programming Language           *
 * Course:                  CPSC 323 - Compilers and Languages                 *
 * Team:                    Cong Le, Dion W Pieterse, San Tran                 *
 * File:                    Lexer.cpp                                          *
 ******************************************************************************/
#include "Lexer.h"

void Lexer::getFile(string &fileName){
    inputStream.open(fileName);
    if(inputStream.is_open())
        cout << "Input file opened." << endl;
    else{
        cerr << "Error: Cannot open input file." << endl;
        exit(EXIT_FAILURE);
    }
    // point to first char in file
    Input_ = &inputStream;
    getTokens();
    inputStream.close();
}


void Lexer::printTokenPairsToConsole(){
    //**-> Output result on the console
    cout<<setw(15)<<left<< "Token: "<<setw(13)<< "Lexemme: "<<setw(10)<< "Line #:"<< "char position:"<<endl;
    cout << string(50, '-') << '\n';
    for(vector<TokenLexPair>::iterator i=list.begin(); i != list.end(); i++){
        cout<<setw(15)<<i->token <<setw(15)<< i->lexeme << setw(13)<< i->lineNum<< i->charNum<<endl;
    }
}


void Lexer::printTokenPairsToTxtFile(){
    outputStream.open("Lexical_Analysis_Result.txt");
    if(outputStream.is_open()) {
        // Formating header for the output file
        cout << "Output file opened." << endl;
        outputStream << left<< setw(25) << "Output:" << '\n';
        outputStream<<setw(15)<<left<< "Token: "<<setw(13)<< "Lexemme: "<<setw(10)<< "Line #:"<< "char position:"<<endl;
        outputStream << string(50, '-') << '\n';

        // Writing legal tokens on to the .txt file
        for(vector<TokenLexPair>::iterator i=list.begin(); i != list.end(); i++){
            outputStream<<setw(15)<<i->token <<setw(15)<< i->lexeme << setw(13)<< i->lineNum<< i->charNum<<endl;
        }
        cout << "Succefully write onto the output .txt file!\n" << endl;
    }
    else {
        cerr << "Error: Cannot Open output file." << endl;
    }
    // close the output text file
    outputStream.close();
}

void Lexer::updateLinePosition(char c){
    if(c == '\n') {
        ++lineNum;
        charNum=1;
    }
    else if( c== '\t') {
        charNum+=3;
    }
    else{
        ++charNum;
    }
}

bool Lexer::isspace_string(std::string s){
    for(int i=0;i<s.size();i++) {
        if(!isspace(s[i])) {
            return false;
        }
    }
    return true;
}
/*=============================================================================
  The implementation of mutator for 2 tables
=============================================================================*/
// Mutator for ID_FSM_TABLE_
void Lexer::ID_FSM(const char &c){
    unsigned int state =(isalpha(c)? 0 : (isdigit(c)? 1 : 2 ));
    // Update new position on the ID_FSM_TABLE_
    IDState_ = ID_FSM_TABLE_[IDState_][state];
}

// Mutator for INT_REAL_FSM_TABLE_
void Lexer::INT_REAL_FSM(const char&c){
    unsigned int state =(isdigit(c) ? 0 :(c == '.'? 1 : 2));
    // Update new position in INT_REAL_FSM_TABLE_
    IntRealState_ = INT_REAL_FSM_TABLE_[IntRealState_][state];
}

/*=============================================================================
  Implementation of helper functions used by both FSMs
=============================================================================*/
// Updating state of lexeme having multiple characrters in FSMs.
void Lexer::updateFSM(const char &c){
    ID_FSM(c);
    INT_REAL_FSM(c);
}
// Peek at next char in input stream
unsigned int Lexer::Peek_ID_FSM(const char& c){
    unsigned int temp_ID_state = IDState_;
    unsigned int state =(isalpha(c)? 0 : (isdigit(c)? 1 : 2 ));
    // Update the new position on the ID_FSM_TABLE_
    return ID_FSM_TABLE_[temp_ID_state][state];
}
// Peeking to the next character in the inputted stream
unsigned int Lexer::Peek_INT_REAL_FSM(const char& c){
    unsigned int temp_INT_REAL_state = IntRealState_;
    unsigned int state =(isdigit(c) ? 0 :(c=='.'? 1 : 2));
    // Update the new position on the INT_REAL_FSM_TABLE_
    return INT_REAL_FSM_TABLE_[temp_INT_REAL_state][state];
}
// Reset all FSMs back to starting state (zero).
void Lexer::resetFSM(){
    IDState_ = 0;
    IntRealState_ = 0;
}
/*=============================================================================
 // The list of implementations of functions
 that check the legality of the inputted characters against the defined sets.
 // Note: The int-return type functions will have the following expecting values
 and their corresponding meanings:
 -1 no match
 0 partial match
 1 perfect match
 //===========================================================================*/
// Is passed string arg in defined keywords?
bool Lexer::isKeyword(const std::string& s){
    // find a match
    std::unordered_set<std::string>::const_iterator search_for = KeywordSet_.find(s);
    if(search_for == KeywordSet_.end()) {
        // if reaches end and nothing is matched
        return false;
    }
    else { return true; }
}
// Is the passed char arg a terminator?
bool Lexer::isTerminator(const char &c){
    return (isSeparator(c)!=-1||isOperator(c)!=-1||isspace(c)!=0||(Peek_ID_FSM(c)==3 && Peek_INT_REAL_FSM(c)==4));
}

// Is passed char arg in defined one-char separators?
int Lexer::isSeparator(const char &c){
    int matcher_state = -1;
    std::string temp;
    // Geting passed character arg
    temp.push_back(c);
    // find match
    for(std::unordered_set<std::string>::const_iterator iter = SeparatorSet_.begin(); iter != SeparatorSet_.end(); iter++){
        if(iter->find(temp)!=std::string::npos){
            if(*iter == temp){
                return 1;
            } else{
                matcher_state = 0;
            }
        }
    }
    // return npos = -1 NO MATCH and 0 if PARTIAL MATCH
    return matcher_state;
}


// Is passed string arg in double-char separators?
int Lexer::isSeparator(const std::string &s){
    int matcher_state = -1;
    for(std::unordered_set<std::string>::const_iterator iter = SeparatorSet_.begin(); iter != SeparatorSet_.end(); iter++){
        if(iter->find(s)!=std::string::npos){
            if(*iter == s){
                // EXACT MATCH for an double-char separator
                return 1;
            } else{
                // PARTIAL MATCH, keep looking for EXACT MATCH
                matcher_state = 0;
            }
        }
    }
    // return npos = -1 if NO MATCH and 0 if PARTIAL MATCH
    return matcher_state;
}
// Is passed char arg in defined one-char operators?
int Lexer::isOperator(const char &c){
    int matcher_state = -1;
    std::string temp;
    // Get passed char arg
    temp.push_back(c);
    // find a match
    for(std::unordered_set<std::string>::const_iterator iter = OperatorSet_.begin(); iter != OperatorSet_.end(); iter++){
        if(iter->find(temp)!= std::string::npos){
            if(*iter == temp) {
                return 1;
            } else {
                // PARTIAL MATCH, keep looking for EXACT MATCH
                matcher_state = 0;
            }
        }
    }
    // return npos = -1 if NO MATCH and 0 if PARTIAL MATCH
    return matcher_state;
}


// Is passed string arg in defined double-char operators?
int Lexer::isOperator(const std::string &s){
    int matcher_state = -1;
    // find a match
    for(std::unordered_set<std::string>::const_iterator iter = OperatorSet_.begin(); iter != OperatorSet_.end(); iter++){
        if(iter->find(s)!=std::string::npos){
            if(*iter == s){
                // EXACT MATCH for an double-char operator
                return 1;
            } else{
                // PARTIAL MATCH, keep looking for potential EXACT MATCH
                matcher_state = 0;
            }
        }
    }
    // return npos = -1 if NO MATCH and 0 if PARTIAL MATCH
    return matcher_state;
}
/*============================================================================
 // The implementation of getPair() that used to read the inputted stream
 of characters and categorize them into valid tokens/lexeme.
 //===========================================================================*/
TokenLexPair Lexer::getPair() {

    char c;
    TokenLexPair output;
    output.lineNum = lineNum;
    output.charNum = charNum;
    // read data file until EOF
    while(Input_->get(c)){
        updateLinePosition(c);
        // If encounter a terminator, and string is empty
        if(!isTerminator(c)){
            // next lexeme we construct will be under the terminator domain
            updateFSM(c);
            // store input char as lexeme into TokenLexPair object
            output.lexeme.push_back(c);
        }
        else if(output.lexeme.empty()){
            std::string potentialSeparator;
            // peek at next char
            char peeker;
            peeker = Input_->peek();
            // Store & build potential two-char separator
            potentialSeparator.push_back(c);
            potentialSeparator.push_back(peeker);
            if(isOperator(c)!= -1||isOperator(potentialSeparator)!= -1){
                output.token="Operator";
                if(isOperator(potentialSeparator) == 1){
                    //we have a two-char operator
                    output.lexeme = potentialSeparator;
                    //take it out of buffer
                    Input_->get(peeker);
                    updateLinePosition(peeker);
                    resetFSM();
                    return output;
                }
                else if(isOperator(c) == 1) {
                    output.lexeme.push_back(c);
                    resetFSM();
                    return output; //it is one-char operator.
                }
                else {
                    //partial match for operator, but never made exact match => unknown
                    output.token = "Unknown";
                    output.lexeme.push_back(c);
                    resetFSM();
                    return output;
                }
            }
            //check for partial match with the separators
            else if(isSeparator(c)!= -1||isSeparator(potentialSeparator)!= -1) {
                //if comment
                if(c=='[' && peeker== '*'){
                    Input_->get(peeker);
                    updateLinePosition(peeker);
                    char last;
                    char secondLast;
                    output.token ="Comment";
                    while(Input_->get(last)){
                        updateLinePosition(last);
                        if(last == '*'){
                            //If are in here last is a '*'
                            secondLast=Input_->peek();
                            //check if char after * is ]
                            if(secondLast==']'){
                                Input_->get(secondLast);
                                updateLinePosition(secondLast);
                                output.token = "Comment";
                                break;
                            }
                        }
                    }
                }
                else if(isSeparator(potentialSeparator) == 1){
                    //we have a 2 character operator
                    output.token = "Separator";
                    output.lexeme = potentialSeparator;
                    //take out of buffer
                    Input_->get(peeker);
                    updateLinePosition(peeker);
                }
                //if single char separator EXACT MATCH
                else if(isSeparator(c) == 1) {
                    //one-char separator checker
                    output.token = "Separator";
                    output.lexeme.push_back(c);
                }
                else{
                    //must be UNKNOWN
                    output.token="Unknown";
                    output.lexeme.push_back(c);
                }
                resetFSM();
                return output;
            }
            else if(isspace(c) != 0){
                // string was empty, read whitespace & ignore
                output.lineNum = lineNum;
                output.charNum = charNum;
            }
            else {
                output.lexeme.push_back(c);
                output.token = "Unknown";
                resetFSM();
                //not a separator, not operator, must be unknown single char ex.) %, or #
                return output;
            }
        }
        else{
            //Is terminator & string not empty. Unget terminator.
            //Check state of finite state machines (break loop that constructs multi-char lexemmes)
            Input_->unget();
            if(c == '\n')
            {
                lineNum--;
            }
            else if(c =='\t')
            {
                charNum-=3;
            }
            else{
                charNum--;
            }
            break;
        }
    }//END WHILE - multi-char lexemmes (3+characters) are constructed, check machines to find what it is
    //============================================================================
    // If at end of stream, and lexemme is empty => EOF reached
    if(Input_->eof() && output.lexeme.empty()) {
        // Stop reading stream. Do nothing.
    }
    //============================================================================
    // Check lexeme against the 2 FSMs
    //============================================================================
    else if(IDState_ == 1 && (IntRealState_ != 1||IntRealState_ != 3)) {
        if(isKeyword(output.lexeme)){
            output.token = "Keyword";
        }
        else{
            output.token = "Identifier";
        }
    }
    else if(IDState_ != 1 && IntRealState_ == 1) {
        output.token = "Int";
    }
    else if(IDState_ != 1 && IntRealState_ == 3) {
        output.token = "Real";
    }
    else{
        //Illegal set of strings
        output.token="Unknown";
    }
    resetFSM();
    return output;
}

/*============================================================================
 // The implementation of getTokens() that used to store data
 into a vector of TokenLexPair objects
 //===========================================================================*/
std::vector<TokenLexPair> Lexer::getTokens(){
    int counter = 0;
    while(!Input_->eof()) {
        TokenLexPair temp = getPair();
        if(temp.token != "Comment"&& !isspace_string(temp.lexeme)) {
            ++counter;
            list.push_back(temp);
        }
    }
    return list;
}
