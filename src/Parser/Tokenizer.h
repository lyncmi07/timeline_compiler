#ifndef MAJOR_TOKENIZER_H
#define MAJOR_TOKENIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Token.h"

class PreProcessedFile;

class Tokenizer {
private:

  //owned memory
  Token* programToken_;

  //not owned
  PreProcessedFile* sourceFile_;

  Symbol currentSymbol_;

  int currentIndex_;
  int currentSymbolLength_;

  int lastErrorSymbol_;
  
  std::string outputString_;

  //moves to the next symbol in the source file
  void nextSymbol();

  //returns true and moves to the next symbol is the current symbol is the same as symbol s
  bool accept(Symbol s);

  //this is called everytime a errorSym token is returned from the non terminal functions. This records the latest point in the source file that the system failed.
  void updateErrorSymbol(int lastSymbolStart);

  //non terminal functions
  Token* program();
  Token* codeBlock();
  Token* line();
  Token* statement();
  Token* variableInitiation();
  Token* variableDefinition();
  Token* expression();
  Token* subExpression();
  Token* equation();
  Token* objectDefinition();
  Token* functionCall();
  Token* parameterSettingStatement();
  Token* lastParameterSetting();
  Token* notLastParameterSetting();
  Token* subVariableIdentifier();
  Token* possibleObject();
  Token* arrayImmediateDefinition();
  Token* classDefinition();
  Token* classCodeBlock();
  Token* methodDefinition();
  Token* parameterDefinitionStatement();
  Token* parameterDefinitionStatementFilled();
  Token* lastParameterDefinition();
  Token* notLastParameterDefinition();
  Token* constructionDefinition();
  Token* floatImmediate();

public:
  //Destructor: destroys programToken_
  ~Tokenizer();
  //Generates a new Tokenizer object and parses the source code of the file given in fileName
  Tokenizer(PreProcessedFile* sourceFile);

  //gets the root token (programToken_)
  Token* programToken() {
    return programToken_;
  }
  
  std::string outputString();
};

#endif
