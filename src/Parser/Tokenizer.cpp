#include "Tokenizer.h"

#include "PreProcessedFile.h"
#include "TerminalToken.h"

#include <iostream>

#include "../Exceptions/CompilerExceptions.h"

using std::cout;
using std::endl;

Tokenizer::Tokenizer(PreProcessedFile* sourceFile) {
  sourceFile_ = sourceFile;

  lastErrorSymbol_ = 0;

  currentIndex_ = -1;

  currentSymbol_ = iterateSymbol;

  while(currentSymbol_ == iterateSymbol) {
    nextSymbol();
  }

  programToken_ = program();

  outputString_ = "";

  if(programToken_->tokenSymbol() == nt_programSym) {
    if(currentIndex_ < sourceFile->fileLength()) {
      std::cout << "parser unsuccessful" << std::endl;
      std::cout << "\033[1;41mERROR:\033[0m parser failed at point: " << sourceFile->getFilePosition(lastErrorSymbol_) << std::endl;
      
      outputString_ += "u:";
      outputString_ += sourceFile->getFilePosition(lastErrorSymbol_);
      
      throw comp_except::TokenizerException();
    } else {
      std::cout << "parser successful" << std::endl;
      outputString_ += "s";
    }
  } else {
    std::cout << "parser unsuccessful" << std::endl;
    std::cout << "\033[1;41mERROR:\033[0m parser failed at point: " << sourceFile->getFilePosition(lastErrorSymbol_) << std::endl;
    
    outputString_ += "u:";
    outputString_ += sourceFile->getFilePosition(lastErrorSymbol_);
    
    throw comp_except::TokenizerException();
  }
}

Tokenizer::~Tokenizer() {
  delete programToken_;
}

std::string Tokenizer::outputString() {
  return outputString_;
}

void Tokenizer::nextSymbol(){ 
  if(currentIndex_ > sourceFile_->fileLength()) {
    currentSymbol_ = eofSymbol;
    return;
  }

  if(currentIndex_ == -1) {
    //put in the first symbol
    currentIndex_ = 0;
  } else {
    //put in subsequent symbols
    currentIndex_ += currentSymbolLength_;
  }

  //TODO: fix so that it is possible to have more than 2 empty characters
  if((*sourceFile_)[currentIndex_] == ' ') {
    //ignore empty space and move on
    currentIndex_++;
    currentSymbolLength_ = 0;
    currentSymbol_ = iterateSymbol;
    return;
  }

  if((*sourceFile_)->substr(currentIndex_).find("<") == 0) {
    //ignore comment and move on
    currentIndex_ += (*sourceFile_)->substr(currentIndex_).find(">") + 1;
    currentSymbolLength_ = 0;
    currentSymbol_ = iterateSymbol;
    return;
  }

 
 //TODO: stop these keywords from blocking identifiers that start with the same word (check if there is a identifier allowable character directly after the word)

  //find keywords first
  if((*sourceFile_)->substr(currentIndex_).find("new") == 0) {
    char nextChar = (*sourceFile_)->substr(currentIndex_+3)[0];
    if((nextChar >= 48 && nextChar <= 57) ||
       (nextChar >= 65 && nextChar <= 90) ||
       (nextChar >= 97 && nextChar <= 122) ||
       (nextChar == 95)) {
    } else {
      currentSymbol_ = newSym;
      currentSymbolLength_ = 3;
      return;
    }
  }

  if((*sourceFile_)->substr(currentIndex_).find("void") == 0) {
    char nextChar = (*sourceFile_)->substr(currentIndex_+4)[0];
    if((nextChar >= 48 && nextChar <= 57) ||
       (nextChar >= 65 && nextChar <= 90) ||
       (nextChar >= 97 && nextChar <= 122) ||
       (nextChar == 95)) {
    } else {
      currentSymbol_ = voidSym;
      currentSymbolLength_ = 4;
      return;
    }
  }
  
  if((*sourceFile_)->substr(currentIndex_).find("_construct") == 0) {
    char nextChar = (*sourceFile_)->substr(currentIndex_+10)[0];
    if((nextChar >= 48 && nextChar <= 57) ||
       (nextChar >= 65 && nextChar <= 90) ||
       (nextChar >= 97 && nextChar <= 122) ||
       (nextChar == 95)) {
    } else {
      currentSymbol_ = constructSym;
      currentSymbolLength_ = 10;
      return;
    }
  }

  if((*sourceFile_)->substr(currentIndex_).find("class") == 0) {
    char nextChar = (*sourceFile_)->substr(currentIndex_+5)[0];
    if((nextChar >= 48 && nextChar <= 57) ||
       (nextChar >= 65 && nextChar <= 90) ||
       (nextChar >= 97 && nextChar <= 122) ||
       (nextChar == 95)) {
    } else {
      currentSymbol_ = classSym;
      currentSymbolLength_ = 5;
      return;
    }
  }

  if((*sourceFile_)->substr(currentIndex_).find("extends") == 0) {
    char nextChar = (*sourceFile_)->substr(currentIndex_+7)[0];
    if((nextChar >= 48 && nextChar <= 57) ||
       (nextChar >= 65 && nextChar <= 90) ||
       (nextChar >= 97 && nextChar <= 122) ||
       (nextChar == 95)) {
    } else {
      currentSymbol_ = extendsSym;
      currentSymbolLength_ = 7;
      return;
    }
  }

  //check for characters next
  if((*sourceFile_)[currentIndex_] == ';') {
     currentSymbol_ = semicolonSym;
     currentSymbolLength_ = 1;
     return;
  }
  if((*sourceFile_)[currentIndex_] == '=') {
    currentSymbol_ = equalsSym;
    currentSymbolLength_ = 1;
    return;
  }
  if((*sourceFile_)[currentIndex_] == '(') {
    currentSymbol_ = openParenthasisSym;
    currentSymbolLength_ = 1;
    return;
  }
  if((*sourceFile_)[currentIndex_] == ')') {
    currentSymbol_ = closeParenthasisSym;
    currentSymbolLength_ = 1;
    return;
  }
  if((*sourceFile_)[currentIndex_] == ',') {
    currentSymbol_ = commaSym;
    currentSymbolLength_ = 1;
    return;
  }
  if((*sourceFile_)[currentIndex_] == '[') {
    currentSymbol_ = openSquareBracketSym;
    currentSymbolLength_ = 1;
    return;
  }
  if((*sourceFile_)[currentIndex_] == ']') {
    currentSymbol_ = closeSquareBracketSym;
    currentSymbolLength_ = 1;
    return;
  }
  if((*sourceFile_)[currentIndex_] == '.') {
    currentSymbol_ = periodSym;
    currentSymbolLength_ = 1;
    return;
  } 
  if((*sourceFile_)[currentIndex_] == '{') {
    currentSymbol_ = openCurlyBracesSym;
    currentSymbolLength_ = 1;
    return;
  }
  if((*sourceFile_)[currentIndex_] == '}') {
    currentSymbol_ = closeCurlyBracesSym;
    currentSymbolLength_ = 1;
    return;
  }
  
  //check for operators
  if((*sourceFile_)[currentIndex_] == '+'
     || (*sourceFile_)[currentIndex_] == '-'
     || (*sourceFile_)[currentIndex_] == '/'
     || (*sourceFile_)[currentIndex_] == '*') {
    currentSymbol_ = operatorSym;
    currentSymbolLength_ = 1;
    return;
  }


  //check for identifierSym
  int characterCheck = currentIndex_;
  if(((*sourceFile_)->substr(characterCheck)[0] >= 65
     && (*sourceFile_)->substr(characterCheck)[0] <= 90) ||
     ((*sourceFile_)->substr(characterCheck)[0] >= 97
     && (*sourceFile_)->substr(characterCheck)[0] <= 122)) {
    characterCheck++;
    while(1==1) {
      if(((*sourceFile_)->substr(characterCheck)[0] >= 65
         && (*sourceFile_)->substr(characterCheck)[0] <= 90) ||
         ((*sourceFile_)->substr(characterCheck)[0] >= 97
         && (*sourceFile_)->substr(characterCheck)[0] <= 122) ||
         ((*sourceFile_)->substr(characterCheck)[0] >= 48
         && (*sourceFile_)->substr(characterCheck)[0] <= 57) ||
         (*sourceFile_)->substr(characterCheck)[0] == '_') {
        characterCheck++;
      } else {
        break;
      }
    }
    currentSymbol_ = identifierSym;
    currentSymbolLength_ = characterCheck - currentIndex_;
    return;
  }

  //check for stringLiteralSym
  if((*sourceFile_)->substr(characterCheck)[0] == '"') {
    characterCheck += (*sourceFile_)->substr(1).substr(characterCheck).find('"');
    currentSymbol_ = stringLiteralSym;
    currentSymbolLength_ = (characterCheck - currentIndex_) + 2;
    return;
  }

  //check for integer
  characterCheck = currentIndex_;
  if((*sourceFile_)->substr(characterCheck)[0] >= 48
     && (*sourceFile_)->substr(characterCheck)[0] <= 57) {
    while(1==1) {
      if((*sourceFile_)->substr(characterCheck)[0] >= 48
         && (*sourceFile_)->substr(characterCheck)[0] <= 57) {
        characterCheck++;
      } else {
        break;
      }
    }

    currentSymbol_ = integerSym;
    currentSymbolLength_ = characterCheck - currentIndex_;
    return;
  }



  currentSymbol_ = errorSym;
}

bool Tokenizer::accept(Symbol s) {
  if(currentSymbol_ == s) {
    currentSymbol_ = iterateSymbol;
    while(currentSymbol_ == iterateSymbol) {
      nextSymbol();
    }
    return true;
  } else {
    return false;
  }
}

void Tokenizer::updateErrorSymbol(int lastSymbolStart) {
  if(lastSymbolStart > lastErrorSymbol_) {
    lastErrorSymbol_ = lastSymbolStart;
  }
}

Token* Tokenizer::program() {
  /*
    <code block> |
    <code block> <program> | 
    <class definition> |
    <class definition> <program>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* codeBlockT = codeBlock();

  Token* programT = nullptr;

  if(codeBlockT->tokenSymbol() == nt_codeBlockSym) {
    programT = program();

    if(programT->tokenSymbol() == nt_programSym) {
      Token* thisToken = new Token(nt_programSym, PROGRAM2, symbolStart);
      thisToken->addNextToken(codeBlockT);
      thisToken->addNextToken(programT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete codeBlockT;
  delete programT;
  programT = nullptr;
  codeBlockT = nullptr;
  
  Token* classDefinitionT = classDefinition();
  
 
  if(classDefinitionT->tokenSymbol() == nt_classDefinitionSym) {
    programT = program();

    if(programT->tokenSymbol() == nt_programSym) {
      Token* thisToken = new Token(nt_programSym, PROGRAM4, symbolStart);
      thisToken->addNextToken(classDefinitionT);
      thisToken->addNextToken(programT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete classDefinitionT;
  delete programT;
  programT = nullptr;
  classDefinitionT = nullptr;
  
  codeBlockT = codeBlock();

  if(codeBlockT->tokenSymbol() == nt_codeBlockSym) {
    Token* thisToken = new Token(nt_programSym, PROGRAM, symbolStart);
    thisToken->addNextToken(codeBlockT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete codeBlockT;
  codeBlockT = nullptr;

  classDefinitionT = classDefinition();
 
  if(classDefinitionT->tokenSymbol() == nt_classDefinitionSym) {
    Token* thisToken = new Token(nt_programSym, PROGRAM3, symbolStart);
    thisToken->addNextToken(classDefinitionT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete classDefinitionT;
  classDefinitionT = nullptr;


  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::codeBlock() {
  /*
    <line> |
    <line> <code block>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* lineT = line();
  Token* codeBlockT = nullptr;
  if(lineT->tokenSymbol() == nt_lineSym) {
    codeBlockT = codeBlock();

    if(codeBlockT->tokenSymbol() == nt_codeBlockSym) {
      Token* thisToken = new Token(nt_codeBlockSym, CODE_BLOCK2, symbolStart);
      thisToken->addNextToken(lineT);
      thisToken->addNextToken(codeBlockT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete lineT;
  delete codeBlockT;
  codeBlockT = nullptr;

  lineT = line();

  if(lineT->tokenSymbol() == nt_lineSym) {
    Token* thisToken = new Token(nt_codeBlockSym, CODE_BLOCK, symbolStart);
    thisToken->addNextToken(lineT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete lineT;
  lineT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}



Token* Tokenizer::line() {
  /*
    <statement> semicolonSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* statementT = statement();
  if(statementT->tokenSymbol() == nt_statementSym) {
    if(accept(semicolonSym)) {
      Token* semicolonT = new TerminalToken(semicolonSym, ";", currentIndex_);
      Token* thisToken = new Token(nt_lineSym, LINE, symbolStart);
      thisToken->addNextToken(statementT);
      thisToken->addNextToken(semicolonT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete statementT;
  statementT = nullptr;
  
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::statement() {
  /*
    <variable definition> |
    <variable initiation> |
    <function call>       |
    <sub variable identifier>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* variableDefinitionT = variableDefinition();
  if(variableDefinitionT->tokenSymbol() == nt_variableDefinitionSym) {
    Token* thisToken = new Token(nt_statementSym, STATEMENT, symbolStart);
    thisToken->addNextToken(variableDefinitionT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete variableDefinitionT;
  variableDefinitionT = nullptr;
  
  Token* variableInitiationT = variableInitiation();
  if(variableInitiationT->tokenSymbol() == nt_variableInitiationSym) {
    Token* thisToken = new Token(nt_statementSym, STATEMENT2, symbolStart);
    thisToken->addNextToken(variableInitiationT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete variableInitiationT;
  variableInitiationT = nullptr;
  
  Token* functionCallT = functionCall();

  if(functionCallT->tokenSymbol() == nt_functionCallSym) {
    Token* thisToken = new Token(nt_statementSym, STATEMENT3, symbolStart);
    thisToken->addNextToken(functionCallT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  
  delete functionCallT;
  functionCallT = nullptr;
  
  Token* subVariableIdentifierT = subVariableIdentifier();
  
  if(subVariableIdentifierT->tokenSymbol() == nt_subVariableIdentifierSym) {
    Token* thisToken = new Token(nt_statementSym, STATEMENT4, symbolStart);
    thisToken->addNextToken(subVariableIdentifierT);
    return thisToken;
  }
  
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::variableInitiation() {
  /*
    identifierSym identifierSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  int symbolStartPrevious = currentIndex_;

  Token* identifierT = nullptr;
  Token* identifierT2 = nullptr;
  if(accept(identifierSym)) {
    //accepted first identifier symbol
    identifierT = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
    symbolStartPrevious = currentIndex_;
    if(accept(identifierSym)) {
      //accepted second identifier symbol
      identifierT2 = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), symbolStartPrevious));

      Token* thisToken = new Token(nt_variableInitiationSym, VARIABLE_INITIATION, symbolStart);
      thisToken->addNextToken(identifierT);
      thisToken->addNextToken(identifierT2);
      return thisToken;
    }
  }
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete identifierT;
  delete identifierT2;
  identifierT = nullptr;
  identifierT2 = nullptr;
  
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::variableDefinition() {
  /*
    <variable initiation> equalsSym <expression> | 
    identifierSym equalsSym <expression> |
    <sub variable identifier> equalsSym <expression>
  */
  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* variableInitiationT = variableInitiation();
  Token* equalsT = nullptr;
  Token* expressionT = nullptr;
  if(variableInitiationT->tokenSymbol() == nt_variableInitiationSym && accept(equalsSym)) {
    equalsT = new TerminalToken(equalsSym, "=", currentIndex_);

    expressionT = expression();

    if(expressionT->tokenSymbol() == nt_expressionSym) {
      Token* thisToken = new Token(nt_variableDefinitionSym, VARIABLE_DEFINITION, symbolStart);
      thisToken->addNextToken(variableInitiationT);
      thisToken->addNextToken(equalsT);
      thisToken->addNextToken(expressionT);
      return thisToken;
    }
  }


  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete variableInitiationT;
  delete equalsT;
  delete expressionT;
  variableInitiationT = nullptr;
  equalsT = nullptr;
  expressionT = nullptr;

  Token* identifierT = nullptr;

  int symbolStartPrevious = currentIndex_;

  if(accept(identifierSym)) {
    identifierT = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
    symbolStartPrevious = currentIndex_;

    if(accept(equalsSym)) {
      equalsT = (new TerminalToken(equalsSym, "=", currentIndex_));
      expressionT = expression();

      if(expressionT->tokenSymbol() == nt_expressionSym) {
        Token* thisToken = new Token(nt_variableDefinitionSym, VARIABLE_DEFINITION2, symbolStart);
        thisToken->addNextToken(identifierT);
        thisToken->addNextToken(equalsT);
        thisToken->addNextToken(expressionT);
        return thisToken;
      }
    }
  }
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete identifierT;
  delete equalsT;
  delete expressionT;
  identifierT = nullptr;
  equalsT = nullptr;
  expressionT = nullptr;
  
  Token* subVariableIdentifierT = subVariableIdentifier();
  if(subVariableIdentifierT->tokenSymbol() == nt_subVariableIdentifierSym) {
    if(accept(equalsSym)){
      equalsT = (new TerminalToken(equalsSym, "=", currentIndex_));
      
      expressionT = expression();
  
      if(expressionT->tokenSymbol() == nt_expressionSym) {
        Token* thisToken = new Token(nt_variableDefinitionSym, VARIABLE_DEFINITION3, symbolStart);
        thisToken->addNextToken(subVariableIdentifierT);
        thisToken->addNextToken(equalsT);
        thisToken->addNextToken(expressionT);
        return thisToken;
      }
    }
  }
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete subVariableIdentifierT;
  delete equalsT;
  delete expressionT;
  subVariableIdentifierT = nullptr;
  equalsT = nullptr;
  expressionT = nullptr;
  
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::expression() {
  /*
    <sub expression> |
    <object definition> |
    <equation>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* equationT= equation();
  if(equationT->tokenSymbol() == nt_equationSym) {
    Token* thisToken = new Token(nt_expressionSym, EXPRESSION3, symbolStart);
    thisToken->addNextToken(equationT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete equationT;
  equationT = nullptr;

  Token* subExpressionT = subExpression();
  if(subExpressionT->tokenSymbol() == nt_subExpressionSym) {
    Token* thisToken = new Token(nt_expressionSym, EXPRESSION, symbolStart);
    thisToken->addNextToken(subExpressionT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete subExpressionT;
  subExpressionT = nullptr;

  Token* objectDefinitionT = objectDefinition();
  
  if(objectDefinitionT->tokenSymbol() == nt_objectDefinitionSym) {
    Token* thisToken = new Token(nt_expressionSym, EXPRESSION2, symbolStart);
    thisToken->addNextToken(objectDefinitionT);
    return thisToken;
  }  

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete objectDefinitionT;
  objectDefinitionT = nullptr;
  
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::subExpression() {
  /*
    identifierSym | 
    integerSym |
    floatSym | 
    stringLiteralSym |
    <sub variable identifier> |
    <function call>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  
  Token* subVariableIdentifierT = subVariableIdentifier();
  if(subVariableIdentifierT->tokenSymbol() == nt_subVariableIdentifierSym) {
    Token* thisToken = new Token(nt_subExpressionSym, SUB_EXPRESSION5, symbolStart);
    thisToken->addNextToken(subVariableIdentifierT);
    return thisToken;
  }
  
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete subVariableIdentifierT;
  subVariableIdentifierT = nullptr;
  
  Token* functionCallT = functionCall();
  if(functionCallT->tokenSymbol() == nt_functionCallSym) {
    Token* thisToken = new Token(nt_subExpressionSym, SUB_EXPRESSION6, symbolStart);
    thisToken->addNextToken(functionCallT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete functionCallT;
  functionCallT = nullptr;

  Token* floatT = floatImmediate();		

  if(floatT->tokenSymbol() == nt_floatImmediateSym) {
    Token* thisToken = new Token(nt_subExpressionSym, SUB_EXPRESSION3, symbolStart);
    thisToken->addNextToken(floatT);
    return thisToken;
  }
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete floatT;
  floatT = nullptr;

  Token* stringLiteralT = nullptr;
  int symbolStartPrevious = currentIndex_;
  int symbolLengthPrevious = currentSymbolLength_;
  if(accept(stringLiteralSym)) {
    stringLiteralT = (new TerminalToken(stringLiteralSym, (*sourceFile_)->substr(symbolStartPrevious, symbolLengthPrevious), currentIndex_));

    Token* thisToken = new Token(nt_subExpressionSym, SUB_EXPRESSION4, symbolStart);
    thisToken->addNextToken(stringLiteralT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete stringLiteralT;
  stringLiteralT = nullptr;

  Token* identifierT = nullptr;
  if(accept(identifierSym)) {
    identifierT = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));

    Token* thisToken = new Token(nt_subExpressionSym, SUB_EXPRESSION, symbolStart);
    thisToken->addNextToken(identifierT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete identifierT;
  identifierT = nullptr;

  Token* integerT = nullptr;

  symbolStartPrevious = currentIndex_;
  if(accept(integerSym)) {
    integerT = (new TerminalToken(integerSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
    
    Token* thisToken = new Token(nt_subExpressionSym, SUB_EXPRESSION2, symbolStart);
    thisToken->addNextToken(integerT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete integerT;
  integerT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::equation() {
  /*
    <sub expression> operatorSym <sub expression> |
    <sub expression> operatorSym <equation>
  */
  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* subExpressionT = subExpression(); 
  Token* operatorT = nullptr;
  Token* equationT = nullptr;

  if(subExpressionT->tokenSymbol() == nt_subExpressionSym) {
    int operatorSymbolStart = currentIndex_;
    if(accept(operatorSym)) {
      //TODO: fix this in case there are spaces between each subExpression
      operatorT = (new TerminalToken(operatorSym, (*sourceFile_)->substr(operatorSymbolStart, 1), currentIndex_));

      equationT = equation();

      if(equationT->tokenSymbol() == nt_equationSym) {
        Token* thisToken = new Token(nt_equationSym, EQUATION2, symbolStart);
        thisToken->addNextToken(subExpressionT);
        thisToken->addNextToken(operatorT);
        thisToken->addNextToken(equationT);
        return thisToken;
      }
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete subExpressionT;
  delete operatorT;
  delete equationT;
  subExpressionT = nullptr;
  operatorT = nullptr;
  equationT = nullptr;

  subExpressionT = subExpression();
  Token* subExpressionT2 = nullptr;
  if(subExpressionT->tokenSymbol() == nt_subExpressionSym) {
    int operatorSymbolStart = currentIndex_;
    if(accept(operatorSym)) {
      //TODO: fix this in case there are spaces between each subExpression
      operatorT = (new TerminalToken(operatorSym, (*sourceFile_)->substr(operatorSymbolStart, 1), currentIndex_));

      subExpressionT2 = subExpression();
      if(subExpressionT2->tokenSymbol() == nt_subExpressionSym) {
        Token* thisToken = new Token(nt_equationSym, EQUATION, symbolStart);
        thisToken->addNextToken(subExpressionT);
        thisToken->addNextToken(operatorT);
        thisToken->addNextToken(subExpressionT2);
        return thisToken;
      }
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete subExpressionT;
  delete operatorT;
  delete equationT;
  delete subExpressionT2;
  subExpressionT = nullptr;
  operatorT = nullptr;
  equationT = nullptr;
  subExpressionT2 = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::objectDefinition() {
  /*
    newSym <function call> <array immediate definition>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* newT = nullptr;
  Token* functionCallT = nullptr;
  Token* arrayImmediateDefinitionT = nullptr;

  if(accept(newSym)) {
    newT = (new TerminalToken(newSym, "new", currentIndex_));
    functionCallT = functionCall();
 
    if(functionCallT->tokenSymbol() == nt_functionCallSym) {
      arrayImmediateDefinitionT = arrayImmediateDefinition();

      if(arrayImmediateDefinitionT->tokenSymbol() == nt_arrayImmediateDefinitionSym) {
        Token* thisToken = new Token(nt_objectDefinitionSym, OBJECT_DEFINITION, symbolStart);
        thisToken->addNextToken(newT);
        thisToken->addNextToken(functionCallT);
        thisToken->addNextToken(arrayImmediateDefinitionT);
        return thisToken;
      } 
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete newT;
  delete functionCallT;
  delete arrayImmediateDefinitionT;
  newT = nullptr;
  functionCallT = nullptr;
  arrayImmediateDefinitionT = nullptr;
  
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::functionCall() {
  /*
    identifierSym openParenthasisSym <parameter setting statement> closeParenthasisSym |
    identifierSym openParenthasisSym closeParenthasisSym
  */
  
  
  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* identifierT = nullptr;
  Token* openParenthasisT = nullptr;
  Token* parameterSettingStatementT = nullptr;
  Token* closeParenthasisT = nullptr;

  int symbolStartPrevious = currentIndex_;
  if(accept(identifierSym)) {
    identifierT = (new TerminalToken(stringLiteralSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));

    symbolStartPrevious = currentIndex_;
    if(accept(openParenthasisSym)) {
      openParenthasisT = (new TerminalToken(openParenthasisSym, "(", currentIndex_));
      parameterSettingStatementT = parameterSettingStatement();
      if(parameterSettingStatementT->tokenSymbol() == nt_parameterSettingStatementSym && accept(closeParenthasisSym)) {
        closeParenthasisT = (new TerminalToken(closeParenthasisSym, ")", currentIndex_));
        Token* thisToken = new Token(nt_functionCallSym, FUNCTION_CALL, symbolStart);
        thisToken->addNextToken(identifierT);
        thisToken->addNextToken(openParenthasisT);
        thisToken->addNextToken(parameterSettingStatementT);
        thisToken->addNextToken(closeParenthasisT);

        return thisToken;
      }
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete identifierT;
  delete openParenthasisT;
  delete parameterSettingStatementT;
  delete closeParenthasisT;
  identifierT = nullptr;
  openParenthasisT = nullptr;
  parameterSettingStatementT = nullptr;
  closeParenthasisT = nullptr;

  symbolStartPrevious = currentIndex_;

  if(accept(identifierSym)) {
    identifierT = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
    
    symbolStartPrevious = currentIndex_;
    if(accept(openParenthasisSym)) {
      openParenthasisT = (new TerminalToken(openParenthasisSym, "(", currentIndex_));
   
      if(accept(closeParenthasisSym)) {  
        closeParenthasisT = (new TerminalToken(closeParenthasisSym, ")", currentIndex_));
        Token* thisToken = new Token(nt_functionCallSym, FUNCTION_CALL2, symbolStart);
        thisToken->addNextToken(identifierT);
        thisToken->addNextToken(openParenthasisT);
        thisToken->addNextToken(closeParenthasisT);
        return thisToken;
      }
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete identifierT;
  delete openParenthasisT;
  delete closeParenthasisT;
  identifierT = nullptr;
  openParenthasisT = nullptr;
  closeParenthasisT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::parameterSettingStatement() {
  /*
    <last parameter setting> |
    <not last parameter setting> <parameter setting statement>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* notLastParameterSettingT = notLastParameterSetting();
  Token* parameterSettingStatementT = nullptr;

  if(notLastParameterSettingT->tokenSymbol() == nt_notLastParameterSettingSym) {
    parameterSettingStatementT = parameterSettingStatement();

    if(parameterSettingStatementT->tokenSymbol() == nt_parameterSettingStatementSym) {
      Token* thisToken = new Token(nt_parameterSettingStatementSym, PARAMETER_SETTING_STATEMENT2, symbolStart);
      thisToken->addNextToken(notLastParameterSettingT);
      thisToken->addNextToken(parameterSettingStatementT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete notLastParameterSettingT;
  delete parameterSettingStatementT;
  notLastParameterSettingT = nullptr;
  parameterSettingStatementT = nullptr;

  Token* lastParameterSettingT = lastParameterSetting();

  if(lastParameterSettingT->tokenSymbol() == nt_lastParameterSettingSym) {
    Token* thisToken = new Token(nt_parameterSettingStatementSym, PARAMETER_SETTING_STATEMENT, symbolStart);
    thisToken->addNextToken(lastParameterSettingT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete lastParameterSettingT;
  lastParameterSettingT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::lastParameterSetting() {
  /*
    <expression>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* expressionT = expression();

  if(expressionT->tokenSymbol() == nt_expressionSym) {
    Token* thisToken = new Token(nt_lastParameterSettingSym, LAST_PARAMETER_SETTING, symbolStart);
    thisToken->addNextToken(expressionT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete expressionT;
  expressionT = nullptr;
 
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart)); 
}

Token* Tokenizer::notLastParameterSetting() {
  /*
    <expression> commaSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* expressionT = expression();
  Token* commaT = nullptr;

  if(expressionT->tokenSymbol() == nt_expressionSym && accept(commaSym)) {
    commaT = (new TerminalToken(commaSym, ",", currentIndex_));
    Token* thisToken = new Token(nt_notLastParameterSettingSym, NOT_LAST_PARAMETER_SETTING, symbolStart);
    thisToken->addNextToken(expressionT);
    thisToken->addNextToken(commaT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete expressionT;
  delete commaT;
  expressionT = nullptr;
  commaT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
} 

Token* Tokenizer::subVariableIdentifier() {
  /*
    <possible object> periodSym <possible object> | 
    <possible object> periodSym <sub variable identifier>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* possibleObjectT = possibleObject();
  Token* periodT = nullptr;
  Token* subVariableIdentifierT = nullptr;
  if(possibleObjectT->tokenSymbol() == nt_possibleObjectSym && accept(periodSym)) {
    periodT = (new TerminalToken(periodSym, ".", currentIndex_));
    subVariableIdentifierT = subVariableIdentifier();

    if(subVariableIdentifierT->tokenSymbol() == nt_subVariableIdentifierSym) {
      Token* thisToken = new Token(nt_subVariableIdentifierSym, SUB_VARIABLE_IDENTIFIER2, symbolStart);
      thisToken->addNextToken(possibleObjectT);
      thisToken->addNextToken(periodT);
      thisToken->addNextToken(subVariableIdentifierT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete possibleObjectT;
  delete periodT;
  delete subVariableIdentifierT;
  possibleObjectT = nullptr;
  periodT = nullptr;
  subVariableIdentifierT = nullptr;
  
  possibleObjectT = possibleObject();
  Token* possibleObjectT2 = nullptr;

  if(possibleObjectT->tokenSymbol() == nt_possibleObjectSym && accept(periodSym)) {
    periodT = (new TerminalToken(periodSym, ".", currentIndex_));
    possibleObjectT2 = possibleObject();
    if(possibleObjectT2->tokenSymbol() == nt_possibleObjectSym) {
      Token* thisToken = new Token(nt_subVariableIdentifierSym, SUB_VARIABLE_IDENTIFIER, symbolStart);
      thisToken->addNextToken(possibleObjectT);
      thisToken->addNextToken(periodT);
      thisToken->addNextToken(possibleObjectT2);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete possibleObjectT;
  delete periodT;
  delete possibleObjectT2;
  possibleObjectT = nullptr;
  periodT = nullptr;
  possibleObjectT2 = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::possibleObject() {
  /*
    identifier |
    <function call>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* functionCallT = functionCall();

  if(functionCallT->tokenSymbol() == nt_functionCallSym) {
    Token* thisToken = new Token(nt_possibleObjectSym, POSSIBLE_OBJECT2, symbolStart);
    thisToken->addNextToken(functionCallT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete functionCallT;
  functionCallT = nullptr;

  Token* identifierT = nullptr;
  
  int symbolStartPrevious = currentIndex_;

  if(accept(identifierSym)) {
    identifierT = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));

    Token* thisToken = new Token(nt_possibleObjectSym, POSSIBLE_OBJECT, symbolStart);
    thisToken->addNextToken(identifierT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete identifierT;
  identifierT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::arrayImmediateDefinition() {
  /*
    openSquareBracketSym <parameter setting statement> closeSquareBracketSym |
    openSquareBracketSym closeSquareBracketSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* openSquareBracketT = nullptr;
  Token* parameterSettingStatementT = nullptr;
  Token* closeSquareBracketT = nullptr;
  
  if(accept(openSquareBracketSym)) {
    openSquareBracketT = (new TerminalToken(openSquareBracketSym, "[", currentIndex_));

    parameterSettingStatementT = parameterSettingStatement();

    if(parameterSettingStatementT->tokenSymbol() == nt_parameterSettingStatementSym && accept(closeSquareBracketSym)) {
      closeSquareBracketT = (new TerminalToken(closeSquareBracketSym, "]", currentIndex_));

      Token* thisToken = new Token(nt_arrayImmediateDefinitionSym, ARRAY_IMMEDIATE_DEFINITION, symbolStart);
      thisToken->addNextToken(openSquareBracketT);
      thisToken->addNextToken(parameterSettingStatementT);
      thisToken->addNextToken(closeSquareBracketT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete openSquareBracketT;
  delete parameterSettingStatementT;
  delete closeSquareBracketT;
  openSquareBracketT = nullptr;
  parameterSettingStatementT = nullptr;
  closeSquareBracketT = nullptr;

  if(accept(openSquareBracketSym) && accept(closeSquareBracketSym)) {
    openSquareBracketT = (new TerminalToken(openSquareBracketSym, "[", currentIndex_));
    closeSquareBracketT = (new TerminalToken(closeSquareBracketSym, "]", currentIndex_));

    Token* thisToken = new Token(nt_arrayImmediateDefinitionSym, ARRAY_IMMEDIATE_DEFINITION2, symbolStart);
    thisToken->addNextToken(openSquareBracketT);
    thisToken->addNextToken(closeSquareBracketT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete openSquareBracketT;
  delete closeSquareBracketT;
  openSquareBracketT = nullptr;
  closeSquareBracketT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::classDefinition() {
  /*
    classSym identifierSym extendsSym identifierSym openCurlyBracesSym <class code block> closeCurlyBracesSym |
    classSym identifierSym extendsSym identifierSym openCurlyBracesSym closeCurlyBracesSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* classT = nullptr;
  Token* identifierT = nullptr;
  Token* extendsT = nullptr;
  Token* identifierT2 = nullptr;
  Token* openCurlyBracesT = nullptr;
  Token* classCodeBlockT = nullptr;
  Token* closeCurlyBracesT = nullptr;

  if(accept(classSym)) {
    classT = (new TerminalToken(classSym, "class", currentIndex_));

    int symbolStartPrevious = currentIndex_;

    if(accept(identifierSym)) {
      identifierT = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
      
      if(accept(extendsSym)) {
        extendsT = (new TerminalToken(extendsSym, "extends", currentIndex_));

        symbolStartPrevious = currentIndex_;

        if(accept(identifierSym)) {
          identifierT2 = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));

          if(accept(openCurlyBracesSym)) {
            openCurlyBracesT = (new TerminalToken(openCurlyBracesSym, "{", currentIndex_));
            classCodeBlockT = classCodeBlock();
            if(classCodeBlockT->tokenSymbol() == nt_classCodeBlockSym && accept(closeCurlyBracesSym)) {
              closeCurlyBracesT = (new TerminalToken(closeCurlyBracesSym, "}", currentIndex_));
              Token* thisToken = new Token(nt_classDefinitionSym, CLASS_DEFINITION, symbolStart);
              thisToken->addNextToken(classT);
              thisToken->addNextToken(identifierT);
              thisToken->addNextToken(extendsT);
              thisToken->addNextToken(identifierT2);
              thisToken->addNextToken(openCurlyBracesT);
              thisToken->addNextToken(classCodeBlockT);
              thisToken->addNextToken(closeCurlyBracesT);
              return thisToken;
            }
          }
        }
      }
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete classT;
  delete identifierT;
  delete extendsT;
  delete identifierT2;
  delete openCurlyBracesT;
  delete classCodeBlockT;
  delete closeCurlyBracesT;
  classT = nullptr;
  identifierT = nullptr;
  extendsT = nullptr;
  identifierT2 = nullptr;
  openCurlyBracesT = nullptr;
  closeCurlyBracesT = nullptr;

  if(accept(classSym)) {
    classT = (new TerminalToken(classSym, "class", currentIndex_));
    int symbolStartPrevious = currentIndex_;

    if(accept(identifierSym)) {
      identifierT = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
      
      if(accept(extendsSym)) {
        extendsT = (new TerminalToken(extendsSym, "extends", currentIndex_));

        symbolStartPrevious = currentIndex_;

        if(accept(identifierSym)) {
          identifierT2 = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));

          if(accept(openCurlyBracesSym) && accept(closeCurlyBracesSym)) {
            openCurlyBracesT = (new TerminalToken(openCurlyBracesSym, "{", currentIndex_));
            closeCurlyBracesT = (new TerminalToken(closeCurlyBracesSym, "}", currentIndex_));

            Token* thisToken = new Token(nt_classDefinitionSym, CLASS_DEFINITION, symbolStart);
            thisToken->addNextToken(classT);
            thisToken->addNextToken(identifierT);
            thisToken->addNextToken(extendsT);
            thisToken->addNextToken(identifierT2);
            thisToken->addNextToken(openCurlyBracesT);
            thisToken->addNextToken(closeCurlyBracesT);
            return thisToken;
          }
        }
      }
    }
  }
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete classT;
  delete identifierT;
  delete extendsT;
  delete identifierT2;
  delete openCurlyBracesT;
  delete closeCurlyBracesT;
  classT = nullptr;
  identifierT = nullptr;
  extendsT = nullptr;
  identifierT2 = nullptr;
  openCurlyBracesT = nullptr;
  closeCurlyBracesT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::classCodeBlock() {
  /*
    <code block> |
    <code block> <class code block> |
    <method definition> |
    <method definition> <class code block> |
    <construction definition> |
    <construction definition> <class code block> |
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;
  Token* codeBlockT = codeBlock();
  Token* classCodeBlockT = nullptr;
  if(codeBlockT->tokenSymbol() == nt_codeBlockSym) {
    classCodeBlockT = classCodeBlock();
    if(classCodeBlockT->tokenSymbol() == nt_classCodeBlockSym) {
      Token* thisToken = new Token(nt_classCodeBlockSym, CLASS_CODE_BLOCK2, symbolStart);
      thisToken->addNextToken(codeBlockT);
      thisToken->addNextToken(classCodeBlockT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete codeBlockT;
  delete classCodeBlockT;
  codeBlockT = nullptr;
  classCodeBlockT = nullptr;
  
  Token* methodDefinitionT = methodDefinition();
  classCodeBlockT = nullptr;


  if(methodDefinitionT->tokenSymbol() == nt_methodDefinitionSym) {
    classCodeBlockT = classCodeBlock();

    if(classCodeBlockT->tokenSymbol() == nt_classCodeBlockSym) {
      Token* thisToken = new Token(nt_classCodeBlockSym, CLASS_CODE_BLOCK4, symbolStart);
      thisToken->addNextToken(methodDefinitionT);
      thisToken->addNextToken(classCodeBlockT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete methodDefinitionT;
  delete classCodeBlockT;
  methodDefinitionT = nullptr;
  classCodeBlockT = nullptr;

  Token* constructionDefinitionT = constructionDefinition();
  if(constructionDefinitionT->tokenSymbol() == nt_constructionDefinitionSym) {
    classCodeBlockT = classCodeBlock();
    if(classCodeBlockT->tokenSymbol() == nt_classCodeBlockSym) {
      Token* thisToken = new Token(nt_classCodeBlockSym, CLASS_CODE_BLOCK6, symbolStart);
      thisToken->addNextToken(constructionDefinitionT);
      thisToken->addNextToken(classCodeBlockT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  delete constructionDefinitionT;
  delete classCodeBlockT;
  constructionDefinitionT = nullptr;
  classCodeBlockT = nullptr;
  
  
  codeBlockT = codeBlock();

  if(codeBlockT->tokenSymbol() == nt_codeBlockSym) {
    Token* thisToken = new Token(nt_classCodeBlockSym, CLASS_CODE_BLOCK, symbolStart);
    thisToken->addNextToken(codeBlockT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete codeBlockT;
  codeBlockT = nullptr;

  methodDefinitionT = methodDefinition();

  if(methodDefinitionT->tokenSymbol() == nt_methodDefinitionSym) {
    Token* thisToken = new Token(nt_classCodeBlockSym, CLASS_CODE_BLOCK3, symbolStart);
    thisToken->addNextToken(methodDefinitionT);
    return thisToken;
  }
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete methodDefinitionT;
  methodDefinitionT = nullptr;

  constructionDefinitionT = constructionDefinition();

  if(constructionDefinitionT->tokenSymbol() == nt_constructionDefinitionSym) {
    Token* thisToken = new Token(nt_classCodeBlockSym, CLASS_CODE_BLOCK5, symbolStart);
    thisToken->addNextToken(constructionDefinitionT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete constructionDefinitionT;
  constructionDefinitionT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::methodDefinition() {
  /*
    identifierSym identifierSym openParenthasisSym <parameter definition statement> closeParenthasisSym openCurlyBracesSym <code block> closeCurlyBracesSym |
    identifierSym identifierSym openParenthasisSym <parameter definition statement> closeParenthasisSym openCurlyBracesSym closeCurlyBracesSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* identifierT1 = nullptr;
  Token* identifierT2 = nullptr;
  Token* openParenthasisT = nullptr;
  Token* parameterDefinitionStatementT = nullptr;
  Token* closeParenthasisT = nullptr;
  Token* openCurlyBracesT = nullptr;
  Token* codeBlockT = nullptr;
  Token* closeCurlyBracesT = nullptr;
  
  int symbolStartPrevious = currentIndex_;
 
  if(accept(identifierSym)) {
    identifierT1 = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
    symbolStartPrevious = currentIndex_;

    if(accept(identifierSym)) {
      identifierT2 = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));

      if(accept(openParenthasisSym)) {
        openParenthasisT = (new TerminalToken(openParenthasisSym, "(", currentIndex_));

        parameterDefinitionStatementT = parameterDefinitionStatement();

        if(parameterDefinitionStatementT->tokenSymbol() == nt_parameterDefinitionStatementSym) {
          if(accept(closeParenthasisSym) && accept(openCurlyBracesSym)) {
            closeParenthasisT = (new TerminalToken(closeParenthasisSym, ")", currentIndex_));
            openCurlyBracesT = (new TerminalToken(openCurlyBracesSym, "{", currentIndex_));
            codeBlockT = codeBlock();

            if(codeBlockT->tokenSymbol() == nt_codeBlockSym && accept(closeCurlyBracesSym)) {
              closeCurlyBracesT = (new TerminalToken(closeCurlyBracesSym, "}", currentIndex_));
              Token* thisToken = new Token(nt_methodDefinitionSym, METHOD_DEFINITION, symbolStart);
              thisToken->addNextToken(identifierT1);
              thisToken->addNextToken(identifierT2);
              thisToken->addNextToken(openParenthasisT);
              thisToken->addNextToken(parameterDefinitionStatementT);
              thisToken->addNextToken(closeParenthasisT);
              thisToken->addNextToken(openCurlyBracesT);
              thisToken->addNextToken(codeBlockT);
              thisToken->addNextToken(closeCurlyBracesT);
              return thisToken;
            }
          }
        }
      }
    }
  }
  
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;
  
  delete identifierT1;
  delete identifierT2;
  delete openParenthasisT;
  delete parameterDefinitionStatementT;
  delete closeParenthasisT;
  delete openCurlyBracesT;
  delete codeBlockT;
  delete closeCurlyBracesT;
  identifierT1 = nullptr;
  identifierT2 = nullptr;
  openParenthasisT = nullptr;
  parameterDefinitionStatementT = nullptr;
  closeParenthasisT = nullptr;
  openCurlyBracesT = nullptr;
  codeBlockT = nullptr;
  closeCurlyBracesT = nullptr;
  
  symbolStartPrevious = currentIndex_;
  
  if(accept(identifierSym)) {
    identifierT1 = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
    symbolStartPrevious = currentIndex_;

    if(accept(identifierSym)) {
      identifierT2 = (new TerminalToken(identifierSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));

      if(accept(openParenthasisSym)) {
        openParenthasisT = (new TerminalToken(openParenthasisSym, "(", currentIndex_));
        parameterDefinitionStatementT = parameterDefinitionStatement();

        if(parameterDefinitionStatementT->tokenSymbol() == nt_parameterDefinitionStatementSym) {
          if(accept(closeParenthasisSym) && accept(openCurlyBracesSym)) {
            closeParenthasisT = (new TerminalToken(closeParenthasisSym, ")", currentIndex_));
            openCurlyBracesT = (new TerminalToken(openCurlyBracesSym, "{", currentIndex_));

            if(accept(closeCurlyBracesSym)) {
              closeCurlyBracesT = (new TerminalToken(closeCurlyBracesSym, "}", currentIndex_));
              Token* thisToken = new Token(nt_methodDefinitionSym, METHOD_DEFINITION, symbolStart);
              thisToken->addNextToken(identifierT1);
              thisToken->addNextToken(identifierT2);
              thisToken->addNextToken(openParenthasisT);
              thisToken->addNextToken(parameterDefinitionStatementT);
              thisToken->addNextToken(closeParenthasisT);
              thisToken->addNextToken(openCurlyBracesT);
              thisToken->addNextToken(closeCurlyBracesT);
              return thisToken;
            }
          }
        }
      }
    }
  }
  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete identifierT1;
  delete identifierT2;
  delete openParenthasisT;
  delete parameterDefinitionStatementT;
  delete closeParenthasisT;
  delete openCurlyBracesT;
  delete closeCurlyBracesT;
  identifierT1 = nullptr;
  identifierT2 = nullptr;
  openParenthasisT = nullptr;
  parameterDefinitionStatementT = nullptr;
  closeParenthasisT = nullptr;
  openCurlyBracesT = nullptr;
  closeCurlyBracesT = nullptr;
  
  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::parameterDefinitionStatement() {
  /*
    voidSym |
    <parameter definition statement filled> |
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* parameterDefinitionStatementFilledT = parameterDefinitionStatementFilled();

  if(parameterDefinitionStatementFilledT->tokenSymbol() == nt_parameterDefinitionStatementFilledSym) {
    Token* thisToken = new Token(nt_parameterDefinitionStatementSym, PARAMETER_DEFINITION_STATEMENT2, symbolStart);
    thisToken->addNextToken(parameterDefinitionStatementFilledT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete parameterDefinitionStatementFilledT;
  parameterDefinitionStatementFilledT = nullptr;

  Token* voidT = nullptr;

  if(accept(voidSym)) {
    voidT = (new TerminalToken(voidSym, "void", currentIndex_));
    Token* thisToken = new Token(nt_parameterDefinitionStatementSym, PARAMETER_DEFINITION_STATEMENT, symbolStart);
    thisToken->addNextToken(voidT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete voidT;
  voidT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::parameterDefinitionStatementFilled() {
  /*
    <last parameter definition> |
    <not last parameter definition> <parameter definition statement filled>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* notLastParameterDefinitionT = notLastParameterDefinition();
  Token* parameterDefinitionStatementFilledT = nullptr;

  if(notLastParameterDefinitionT->tokenSymbol() == nt_notLastParameterDefinitionSym) {
    parameterDefinitionStatementFilledT = parameterDefinitionStatementFilled();
    if(parameterDefinitionStatementFilledT->tokenSymbol() == nt_parameterDefinitionStatementFilledSym) {
      Token* thisToken = new Token(nt_parameterDefinitionStatementFilledSym, PARAMETER_DEFINITION_STATEMENT_FILLED2, symbolStart);
      thisToken->addNextToken(notLastParameterDefinitionT);
      thisToken->addNextToken(parameterDefinitionStatementFilledT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete notLastParameterDefinitionT;
  delete parameterDefinitionStatementFilledT;
  notLastParameterDefinitionT = nullptr;
  parameterDefinitionStatementFilledT = nullptr;
  
  Token* lastParameterDefinitionT = lastParameterDefinition();
  if(lastParameterDefinitionT->tokenSymbol() == nt_lastParameterDefinitionSym) {
    Token* thisToken = new Token(nt_parameterDefinitionStatementFilledSym, PARAMETER_DEFINITION_STATEMENT_FILLED, symbolStart);
    thisToken->addNextToken(lastParameterDefinitionT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete lastParameterDefinitionT;
  lastParameterDefinitionT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::lastParameterDefinition() {
  /*
    <variable initiation>
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* variableInitiationT = variableInitiation();

  if(variableInitiationT->tokenSymbol() == nt_variableInitiationSym) {
    Token* thisToken = new Token(nt_lastParameterDefinitionSym, LAST_PARAMETER_DEFINITION, symbolStart);
    thisToken->addNextToken(variableInitiationT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete variableInitiationT;
  variableInitiationT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::notLastParameterDefinition() {
  /*
    <variable initiation> commaSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* variableInitiationT = variableInitiation();
  Token* commaT = nullptr;

  if(variableInitiationT->tokenSymbol() == nt_variableInitiationSym && accept(commaSym)) {
    commaT = (new TerminalToken(commaSym, ",", currentIndex_));
    Token* thisToken = new Token(nt_notLastParameterDefinitionSym, NOT_LAST_PARAMETER_DEFINITION, symbolStart);
    thisToken->addNextToken(variableInitiationT);
    thisToken->addNextToken(commaT);
    return thisToken;
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete variableInitiationT;
  delete commaT;
  variableInitiationT = nullptr;
  commaT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::constructionDefinition() {
  /*
    constructSym openParenthasisSym <parameter definition statement> closeParenthasisSym openCurlyBracesSym <code block> closeCurlyBracesSym |
    constructSym openParenthasisSym <parameter definition statement> closeParenthasisSym openCurlyBracesSym closeCurlyBracesSym
  */
  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* constructT = nullptr;
  Token* openParenthasisT = nullptr;
  Token* parameterDefinitionStatementT = nullptr;
  Token* closeParenthasisT = nullptr;
  Token* openCurlyBracesT = nullptr;
  Token* codeBlockT = nullptr;
  Token* closeCurlyBracesT = nullptr;

  if(accept(constructSym) && accept(openParenthasisSym)) {
    constructT = (new TerminalToken(constructSym, "_construct", currentIndex_));
    openParenthasisT = (new TerminalToken(openParenthasisSym, "(", currentIndex_));
    parameterDefinitionStatementT = parameterDefinitionStatement();

    if(parameterDefinitionStatementT->tokenSymbol() == nt_parameterDefinitionStatementSym && accept(closeParenthasisSym) && accept(openCurlyBracesSym)) {
      closeParenthasisT = (new TerminalToken(closeParenthasisSym, ")", currentIndex_));
      openCurlyBracesT = (new TerminalToken(openCurlyBracesSym, "{", currentIndex_));
      codeBlockT = codeBlock();

      if(codeBlockT->tokenSymbol() == nt_codeBlockSym && accept(closeCurlyBracesSym)) {
        closeCurlyBracesT = (new TerminalToken(closeCurlyBracesSym, "}", currentIndex_));
        Token* thisToken = new Token(nt_constructionDefinitionSym, CONSTRUCTION_DEFINITION, symbolStart);
        thisToken->addNextToken(constructT);
        thisToken->addNextToken(openParenthasisT);
        thisToken->addNextToken(parameterDefinitionStatementT);
        thisToken->addNextToken(closeParenthasisT);
        thisToken->addNextToken(openCurlyBracesT);
        thisToken->addNextToken(codeBlockT);
        thisToken->addNextToken(closeCurlyBracesT);
        return thisToken;
      }
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete constructT;
  delete openParenthasisT;
  delete parameterDefinitionStatementT;
  delete closeParenthasisT;
  delete openCurlyBracesT;
  delete codeBlockT;
  delete closeCurlyBracesT;
  constructT = nullptr;
  openParenthasisT = nullptr;
  parameterDefinitionStatementT = nullptr;
  closeParenthasisT = nullptr;
  openCurlyBracesT = nullptr;
  codeBlockT = nullptr;
  closeCurlyBracesT = nullptr;

  if(accept(constructSym) && accept(openParenthasisSym)) {
    constructT = (new TerminalToken(constructSym, "_construct", currentIndex_));
    openParenthasisT = (new TerminalToken(openParenthasisSym, "(", currentIndex_));
    parameterDefinitionStatementT = parameterDefinitionStatement();

    if(parameterDefinitionStatementT->tokenSymbol() == nt_parameterDefinitionStatementSym && accept(closeParenthasisSym) && accept(openCurlyBracesSym) && accept(closeCurlyBracesSym)) {
      closeParenthasisT = (new TerminalToken(closeParenthasisSym, ")", currentIndex_));
      openCurlyBracesT = (new TerminalToken(openCurlyBracesSym, "{", currentIndex_));
      closeCurlyBracesT = (new TerminalToken(closeCurlyBracesSym, "}", currentIndex_));
      Token* thisToken = new Token(nt_constructionDefinitionSym, CONSTRUCTION_DEFINITION2, symbolStart);
      thisToken->addNextToken(constructT);
      thisToken->addNextToken(openParenthasisT);
      thisToken->addNextToken(parameterDefinitionStatementT);
      thisToken->addNextToken(closeParenthasisT);
      thisToken->addNextToken(openCurlyBracesT);
      thisToken->addNextToken(closeCurlyBracesT);
      return thisToken;
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete constructT;
  delete openParenthasisT;
  delete parameterDefinitionStatementT;
  delete closeParenthasisT;
  delete openCurlyBracesT;
  delete closeCurlyBracesT;
  constructT = nullptr;
  openParenthasisT = nullptr;
  parameterDefinitionStatementT = nullptr;
  closeParenthasisT = nullptr;
  openCurlyBracesT = nullptr;
  closeCurlyBracesT = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}

Token* Tokenizer::floatImmediate() {
  /*
    integerSym periodSym integerSym
  */

  int symbolStart = currentIndex_;
  Symbol symbolValue = currentSymbol_;
  int symbolEnd = currentSymbolLength_;

  Token* integerT1 = nullptr;
  Token* periodT = nullptr;
  Token* integerT2 = nullptr;
  
  int symbolStartPrevious = currentIndex_;
  if(accept(integerSym)) {
    integerT1 = (new TerminalToken(integerSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));



    if(accept(periodSym)) {
      periodT = (new TerminalToken(periodSym, ".", currentIndex_));

      symbolStartPrevious = currentIndex_;

      if(accept(integerSym)) {
        integerT2 = (new TerminalToken(integerSym, (*sourceFile_)->substr(symbolStartPrevious, currentIndex_ - symbolStartPrevious), currentIndex_));
        Token* thisToken = new Token(nt_floatImmediateSym, FLOAT_IMMEDIATE, symbolStart);
        thisToken->addNextToken(integerT1);
        thisToken->addNextToken(periodT);
        thisToken->addNextToken(integerT2);
        return thisToken;
      }
    }
  }

  currentIndex_ = symbolStart;
  currentSymbol_ = symbolValue;
  currentSymbolLength_ = symbolEnd;

  delete integerT1;
  delete integerT2;
  integerT1 = nullptr;
  integerT2 = nullptr;

  updateErrorSymbol(symbolStart);
  return (new Token(errorSym, TERMINAL, symbolStart));
}
