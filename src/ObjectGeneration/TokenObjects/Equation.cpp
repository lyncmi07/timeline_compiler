#include "Equation.h"
#include <iostream>
#include "../../Parser/PreProcessedFile.h"
#include "SubExpression.h"
#include "../../Variables/ReturnableValue.h"
#include "../../Variables/ValueManager.h"
#include "../../Exceptions/CompilerExceptions.h"

Equation::Equation(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, SubExpression* subExpression1, std::string givenOperator,  SubExpression* subExpression2)
  : TokenObject(sourceFileIndex) {
  equationPieces_.push_back(subExpression1);
  if(givenOperator.compare("-") == 0) {
    equationOperators_.push_back(MINUS);
  } else if(givenOperator.compare("+") == 0) {
    equationOperators_.push_back(PLUS);
  } else if(givenOperator.compare("*") == 0) {
    equationOperators_.push_back(MULTIPLY);
  } else if(givenOperator.compare("/") == 0) {
    equationOperators_.push_back(DIVIDE);
  } else {
    std::cout << "COMPILER ERROR: contact compiler developer Equation.cpp line 19" << std::endl;
    std::cout << "op given: '" << givenOperator << "'" << std::endl;
  }
  //equationOperators_.push_back(givenOperator);
  equationPieces_.push_back(subExpression2);
  sourceFile_ = sourceFile;
  valueManager_ = valueManager;
}

Equation::Equation(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, SubExpression* subExpression, std::string givenOperator,  Equation* equation)
  : TokenObject(sourceFileIndex) {
  equationPieces_.push_back(subExpression);
  if(givenOperator.compare("-") == 0) {
    equationOperators_.push_back(MINUS);
  } else if(givenOperator.compare("+") == 0) {
    equationOperators_.push_back(PLUS);
  } else if(givenOperator.compare("*") == 0) {
    equationOperators_.push_back(MULTIPLY);
  } else if(givenOperator.compare("/") == 0) {
    equationOperators_.push_back(DIVIDE);
  } else {
    std::cout << "COMPILER ERROR: contact compiler developer Equation.cpp line 40" << std::endl;
  }
  
  for(int i = 0; i < equation->equationPieces()->size(); i++) {
    equationPieces_.push_back(equation->equationPieces()->at(i));
  }
  //clear so that it does not delete the equationPieces
  equation->equationPieces()->clear();
  for(int i = 0; i < equation->equationOperators()->size(); i++) {
    equationOperators_.push_back(equation->equationOperators()->at(i));
  }
  
  sourceFile_ = sourceFile;
  valueManager_ = valueManager;
  
  delete equation;
}

Equation::Equation(const Equation &original)
 : TokenObject(original.sourceFileIndex_) {
  sourceFile_ = original.sourceFile_;
  
  for(int i = 0; i < original.equationPieces_.size(); i++) {
    equationPieces_.push_back(new SubExpression(*(original.equationPieces_[i])));
  }
  for(int i = 0; i < original.equationOperators_.size(); i++) {
    equationOperators_.push_back(original.equationOperators_[i]);
  }
}

Equation::~Equation() {
  for(int i = 0; i < equationPieces_.size(); i++) {
    delete equationPieces_[i];
  }
}

std::vector<SubExpression*>* Equation::equationPieces() {
  return &equationPieces_;
}

std::vector<Operator>* Equation::equationOperators() {
  return &equationOperators_;
}

ReturnableValue* Equation::returningCode() {

  //TODO: fix memory leak. don't know whether this value is coming from existing variable or being generated just now
  //TODO: implement BIDMAS
  //act differently depending on what the type of the first SubExpression value
  
  ReturnableValue* originalValue = equationPieces_[0]->returningCode();
  if(originalValue->valueType().compare("string") == 0) {
    std::string totalString = originalValue->stringLiteralValue();
    //TODO: fix memory leak. don't know whether this value is coming from existing variable or being generated just now
    //this equation is being used to contatonate strings
    //adding integers or floats will have them converted into strings and added.
    
    
    
    for(int i = 1; i < equationPieces_.size(); i++) {
      ReturnableValue* nextValue = equationPieces_[i]->returningCode();
      //TODO: check the operators to make sure that they are always the '+' operator
      if(equationOperators_[i-1] != PLUS) {
        std::cout << "\033[1;41mERROR:\033[0m Only operator to be used in string concatenation is '+': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
      if(nextValue->valueType().compare("string") == 0) {
        totalString += nextValue->stringLiteralValue();
      } else if(nextValue->valueType().compare("int") == 0) {
        totalString += std::to_string(nextValue->integerValue());
      } else if(nextValue->valueType().compare("float") == 0) {
        totalString += std::to_string(nextValue->floatValue());
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Only types allowed in string concatenation are string, int and float: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
      //TODO: fix memory leak. don't know whether this value is coming from existing variable or being generated just now
    }
    
    //return new ReturnableValue(totalString);
    return valueManager_->newValue(totalString);
  } else if(originalValue->valueType().compare("int") == 0) {
    //adding only integers and will return an integer value.
    //if float found then fail.
    int total = originalValue->integerValue();
    for(int i = 1; i < equationPieces_.size(); i++) {
      
      ReturnableValue* nextValue = equationPieces_[i]->returningCode();
      if(nextValue->valueType().compare("int") == 0) {
        switch(equationOperators_[i-1]) {
        case MINUS:
          total -= nextValue->integerValue();
          break;
        case PLUS:
          total += nextValue->integerValue();
          break;
        case MULTIPLY:
          total = total * nextValue->integerValue();
          break;
        case DIVIDE:
          if(nextValue->integerValue() == 0) {
            std::cout << "\033[1;41mERROR:\033[0m Division by 0: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
          } else {
            total /= nextValue->integerValue();
          }
        }
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Only type allowed in integer equation are int: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
    }
    std::cout << "WARNING: Check operator order. This compiler calculates operators in the order that they appear: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    
    //return new ReturnableValue(total);
    return valueManager_->newValue(total);
  } else if(originalValue->valueType().compare("float") == 0) {
  
    //TODO: allow equations that start with an integer and then the subsequent numbers being floats to be processed
  
    //adding integers or floats.
    float total = originalValue->floatValue();
    for(int i = 1; i < equationPieces_.size(); i++) {
      ReturnableValue* nextValue = equationPieces_[i]->returningCode();
      if(nextValue->valueType().compare("int") == 0) {
        switch(equationOperators_[i-1]) {
        case MINUS:
          total -= nextValue->integerValue();
          break;
        case PLUS:
          total += nextValue->integerValue();
          break;
        case MULTIPLY:
          total = total * nextValue->integerValue();
          break;
        case DIVIDE:
          total /= nextValue->integerValue();
        }
      } else if(nextValue->valueType().compare("float") == 0) {
        switch(equationOperators_[i-1]) {
        case MINUS:
          total -= nextValue->floatValue();
          break;
        case PLUS:
          total += nextValue->floatValue();
          break;
        case MULTIPLY:
          total = total * nextValue->floatValue();
          break;
        case DIVIDE:
          total /= nextValue->floatValue();
          break;
        }
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Only types allowed in float equation are int and float: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
    }
    
    std::cout << "WARNING: Check operator order. This compiler calculates operators in the order that they apprear: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    //return new ReturnableValue(total);
    return valueManager_->newValue(total);
  } else {
    std::cout << "\033[1;41mERROR:\033[0m Only types string, int and float can appear in equations: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::StructureException();
  }
}

ReturnableValue* Equation::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  //TODO: fix memory leak. don't know whether this value is coming from existing variable or being generated just now
  //TODO: implement BIDMAS
  //act differently depending on what the type of the first SubExpression value
  
  ReturnableValue* originalValue = equationPieces_[0]->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
  if(originalValue->valueType().compare("string") == 0) {
    std::string totalString = originalValue->stringLiteralValue();
    //TODO: fix memory leak. don't know whether this value is coming from existing variable or being generated just now
    //this equation is being used to contatonate strings
    //adding integers or floats will have them converted into strings and added.
    
    
    for(int i = 1; i < equationPieces_.size(); i++) {
      ReturnableValue* nextValue = equationPieces_[i]->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
      //TODO: check the operators to make sure that they are always the '+' operator
      if(equationOperators_[i-1] != PLUS) {
        std::cout << "\033[1;41mERROR:\033[0m Only operator to be used in string concatenation is '+': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
      if(nextValue->valueType().compare("string") == 0) {
        totalString += nextValue->stringLiteralValue();
      } else if(nextValue->valueType().compare("int") == 0) {
        totalString += std::to_string(nextValue->integerValue());
      } else if(nextValue->valueType().compare("float") == 0) {
        totalString += std::to_string(nextValue->floatValue());
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Only types allowed in string concatenation are string, int and float: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
      //TODO: fix memory leak. don't know whether this value is coming from existing variable or being generated just now
    }
    
    //return new ReturnableValue(totalString);
    return valueManager_->newValue(totalString);
  } else if(originalValue->valueType().compare("int") == 0) {
    //adding only integers and will return an integer value.
    //if float found then fail.
    int total = originalValue->integerValue();
    for(int i = 1; i < equationPieces_.size(); i++) {
      
      ReturnableValue* nextValue = equationPieces_[i]->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
      if(nextValue->valueType().compare("int") == 0) {
        switch(equationOperators_[i-1]) {
        case MINUS:
          total -= nextValue->integerValue();
          break;
        case PLUS:
          total += nextValue->integerValue();
          break;
        case MULTIPLY:
          total = total * nextValue->integerValue();
          break;
        case DIVIDE:
          if(nextValue->integerValue() == 0) {
            std::cout << "\033[1;41mERROR:\033[0m Division by 0: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
            throw comp_except::DivideByZeroException();
          } else {
            total /= nextValue->integerValue();
          }
        }
      } else {
        std::cout << "ERROR: Only type allowed in integer equation are int: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        return nullptr;
      }
    }
    std::cout << "WARNING: Check operator order. This compiler calculates operators in the order that they appear: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    
    //return new ReturnableValue(total);
    return valueManager_->newValue(total);
  } else if(originalValue->valueType().compare("float") == 0) {
  
    //TODO: allow equations that start with an integer and then the subsequent numbers being floats to be processed
  
    //adding integers or floats.
    float total = originalValue->floatValue();
    for(int i = 1; i < equationPieces_.size(); i++) {
      ReturnableValue* nextValue = equationPieces_[i]->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
      if(nextValue->valueType().compare("int") == 0) {
        switch(equationOperators_[i-1]) {
        case MINUS:
          total -= nextValue->integerValue();
          break;
        case PLUS:
          total += nextValue->integerValue();
          break;
        case MULTIPLY:
          total = total * nextValue->integerValue();
          break;
        case DIVIDE:
          total /= nextValue->integerValue();
        }
      } else if(nextValue->valueType().compare("float") == 0) {
        switch(equationOperators_[i-1]) {
        case MINUS:
          total -= nextValue->floatValue();
          break;
        case PLUS:
          total += nextValue->floatValue();
          break;
        case MULTIPLY:
          total = total * nextValue->floatValue();
          break;
        case DIVIDE:
          total /= nextValue->floatValue();
          break;
        }
      } else {
        std::cout << "ERROR: Only types allowed in float equation are int and float: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        return nullptr;
      }
    }
    
    std::cout << "WARNING: Check operator order. This compiler calculates operators in the order that they apprear: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    //return new ReturnableValue(total);
    return valueManager_->newValue(total);
  } else {
    std::cout << "ERROR: Only types string, int and float can appear in equations: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    return nullptr;
  }
}



