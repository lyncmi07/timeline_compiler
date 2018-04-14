#ifndef MAIN_TERMINAL_TOKEN_H
#define MAIN_TERMINAL_TOKEN_H

#include "Token.h"
#include <string>
#include <iostream>
#include <stdlib.h>

class TerminalToken : public Token {
private:
  std::string value_;
public:
  //generates a new Token object for terminal tokens
  //tokenSymbol: the type of symbol this token is
  //value: what is written in the symbol
  //startIndex: the index in the source code that this symbol occurs
  TerminalToken(Symbol tokenSymbol, std::string value, int startIndex);
  std::string getValue();
  
  //treats the value as an identifier. returns the value as given.
  std::string getIdentifier();
  
  //treats the value as a string therefore removing the 2 quote marks from the front and back 
  //of the string
  std::string getStringLiteral();
  
  //This converts the value to an integer. NO ERROR CHECKING IS DONE.
  //If this function is used it is expected for the programmer to 
  //ensure that the value is an integer
  int getInteger();
};

#endif
