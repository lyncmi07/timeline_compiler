#include "Token.h"

Token::~Token() {
  for(int i = 0; i < subTokens_.size(); i++) {
    delete subTokens_[i];
  }
}

Token::Token(Symbol tokenSymbol, SyntaxRule tokenRule, int startIndex) {
  tokenSymbol_ = tokenSymbol;
  tokenRule_ = tokenRule;
  startIndex_ = startIndex;
}


bool Token::isNonTerminal() {
  if(tokenRule_ != TERMINAL) {
    return true;
  } else {
    return false;
  }
}

void Token::addNextToken(Token* nextToken) {
  subTokens_.push_back(nextToken);
}

Token* Token::operator[](int i) {
  return subTokens_[i];
}
