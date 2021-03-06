#include "Statement.h"

#include "VariableDefinition.h"
#include "VariableInitiation.h"
#include "FunctionCall.h"
#include "SubVariableIdentifier.h"
#include <iostream>


Statement::Statement(int sourceFileIndex, VariableDefinition* variableDefinition)
  : TokenObject(sourceFileIndex) {
  type_ = ST_VD;
  variableDefinition_ = variableDefinition;
  variableInitiation_ = nullptr;
  functionCall_ = nullptr;
  subVariableIdentifier_ = nullptr;
}

Statement::Statement(int sourceFileIndex, VariableInitiation* variableInitiation)
  : TokenObject(sourceFileIndex) {
  type_ = ST_VI;
  variableDefinition_ = nullptr;
  variableInitiation_ = variableInitiation;
  functionCall_ = nullptr;
  subVariableIdentifier_ = nullptr;
}

Statement::Statement(int sourceFileIndex, FunctionCall* functionCall)
  : TokenObject(sourceFileIndex) {
  type_ = ST_FC;
  variableDefinition_ = nullptr;
  variableInitiation_ = nullptr;
  functionCall_ = functionCall;
  subVariableIdentifier_ = nullptr;
}

Statement::Statement(int sourceFileIndex, SubVariableIdentifier* subVariableIdentifier) 
  : TokenObject(sourceFileIndex) {
  type_ = ST_SVI;
  variableDefinition_ = nullptr;
  variableInitiation_ = nullptr;
  functionCall_ = nullptr;
  subVariableIdentifier_ = subVariableIdentifier; 
}

Statement::Statement(const Statement &original)
 : TokenObject(original.sourceFileIndex_) {

  type_ = original.type_;

  switch(type_) {
  case ST_VD: 
    variableDefinition_ = new VariableDefinition(*(original.variableDefinition_));
    variableInitiation_ = nullptr;
    functionCall_ = nullptr;
    break;
  case ST_VI:
    variableDefinition_ = nullptr;
    variableInitiation_ = new VariableInitiation(*(original.variableInitiation_));
    functionCall_ = nullptr;
    break;
  case ST_FC:
    variableDefinition_ = nullptr;
    variableInitiation_ = nullptr;
    functionCall_ = new FunctionCall(*(original.functionCall_));
    break;
  }
}

Statement::~Statement() {
  delete variableDefinition_;
  delete variableInitiation_;
  delete functionCall_;
}

void Statement::runCode() {
  switch(type_) {
  case ST_VD:
    variableDefinition_->runCode();
    break;
  case ST_VI:
    variableInitiation_->runCode();
    break;
  case ST_FC:
    functionCall_->runCode();
    break;
  case ST_SVI:
    subVariableIdentifier_->runCode();
    break;
  }
}

void Statement::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  switch(type_) {
  case ST_VD:
    variableDefinition_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    break;
  case ST_VI:
    variableInitiation_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    break;
  case ST_FC:
    functionCall_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    break;
  case ST_SVI:
    subVariableIdentifier_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    break;
  }
}

ReturnableValue* Statement::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  switch(type_) {
  case ST_VD:
    variableDefinition_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    break;
  case ST_VI:
    variableInitiation_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    break;
  case ST_FC:
    if(functionCall_->functionName().compare("return") == 0) {
      return functionCall_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    } else {
      functionCall_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    }
    break;
  case ST_SVI:
    subVariableIdentifier_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    break;
  }
  return nullptr;
}
