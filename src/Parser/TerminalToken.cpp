#include "TerminalToken.h"
#include <iostream>
TerminalToken::TerminalToken(Symbol tokenSymbol, std::string value, int startIndex) : Token(tokenSymbol, TERMINAL, startIndex) {
  value_ = value;
}

std::string TerminalToken::getValue() {
  return value_;
}

std::string TerminalToken::getIdentifier() {
  return value_;
}

std::string TerminalToken::getStringLiteral() {
  return value_.substr(1, value_.length()-2);
}

int TerminalToken::getInteger() {
  int i = atoi(value_.c_str());
  
  return i;
}
