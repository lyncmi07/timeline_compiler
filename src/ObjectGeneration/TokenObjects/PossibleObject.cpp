#include "PossibleObject.h"
#include "FunctionCall.h"
#include "../../Variables/Variable.h"
#include "../../Variables/VariableManager.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Exceptions/CompilerExceptions.h"
#include <iostream>

PossibleObject::PossibleObject(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, std::string identifier)
  : TokenObject(sourceFileIndex) {
  type_ = PO_I;
  identifier_ = identifier;
  variableManager_ = variableManager;
  sourceFile_ = sourceFile;
}

PossibleObject::PossibleObject(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, FunctionCall* functionCall)
  : TokenObject(sourceFileIndex) {
  type_ = PO_FC;
  functionCall_ = functionCall;
  variableManager_ = variableManager;
  sourceFile_ = sourceFile;
}

PossibleObject::PossibleObject(const PossibleObject &original)
  : TokenObject(original.sourceFileIndex_) {
  identifier_ = original.identifier_;
  if(original.functionCall_ != nullptr) {
    functionCall_ = new FunctionCall(*original.functionCall_);
  }
}

PossibleObject::~PossibleObject() {
  delete functionCall_;
}

ReturnableValue* PossibleObject::returningCode() {
  if(type_ == PO_I) {
    Variable* variable = variableManager_->getVariable(identifier_);
    
    if(variable == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m " << identifier_ << "is not a variable: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    
    return variable->value();
    
  } else if(functionCall_->functionName().compare("GetEventContainingDataValue") == 0){
    return functionCall_->returningCode();
  } else {
    std::cout << "\033[1;41mERROR:\033[0m Function " << functionCall_->functionName() << "(...) does not have sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::VariableNotFoundException();
  }
}

ReturnableValue* PossibleObject::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  if(type_ == PO_I) {
    Variable* variable = classVariableManager->getVariable(identifier_);
    
    if(variable == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m " << identifier_ << " is not a sub variable: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    return variable->value();
  } else {
    return functionCall_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
  }
}

ReturnableValue* PossibleObject::returningCodeSeparateClassMode(VariableManager* separateClassVariableManager,
                                                                VariableManager* classVariableManager,
                                                                MethodManager* classMethodManager,
                                                                CreatedClass* parentClass) {
  if(type_ == PO_I) {
    Variable* variable = classVariableManager->getVariable(identifier_);
    
    if(variable == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m " << identifier_ << " is not a sub variable: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    return variable->value();
  } else {
    return functionCall_->returningCodeSeparateClassMode(separateClassVariableManager, classVariableManager, classMethodManager, parentClass);
  }
}

ReturnableValue* PossibleObject::returningCodeOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  if(type_ == PO_I) {
    Variable* variable = classVariableManager->getVariable(identifier_);
    
    if(variable == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m " << identifier_ << " is not a sub variable: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    return variable->value();
  } else {
    return functionCall_->returningCodeOutsideClassMode(classVariableManager, classMethodManager, parentClass);
  }
}

Variable* PossibleObject::getVariable(VariableManager* classVariableManager) {
  if(type_ == PO_I) {
    Variable* variable = classVariableManager->getVariable(identifier_);
    
    if(variable == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m " << identifier_ << " is not a sub variable: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    } else {
      return variable;
    }
  } else {
    std::cout << "\033[1;41mERROR:\033[0m Functions can not be redefined: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::VariableNotFoundException();
  }
}

