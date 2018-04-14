#include "SubExpression.h"
#include "FloatImmediate.h"
#include "SubVariableIdentifier.h"
#include "FunctionCall.h"
#include "../../Variables/VariableManager.h"
#include "../../Variables/Variable.h"
#include "../../Variables/ReturnableValue.h"
#include "../../Variables/ValueManager.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Exceptions/CompilerExceptions.h"
#include <iostream>

SubExpression::SubExpression(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, ValueManager* valueManager, std::string identifier)
  : TokenObject(sourceFileIndex) {
  type_ = SE_ID;
  identifier_ = identifier;
  integerValue_ = -1;
  floatValue_ = -1;
  stringLiteral_ = "";
  subVariableIdentifier_ = nullptr;
  functionCall_ = nullptr;
  variableManager_ = variableManager;
  sourceFile_ = sourceFile;
  valueManager_ = valueManager;
}

SubExpression::SubExpression(int sourceFileIndex, ValueManager* valueManager, int integerValue) 
  : TokenObject(sourceFileIndex) {
  type_ = SE_INT;
  identifier_ = "";
  integerValue_ = integerValue;
  floatValue_ = -1;
  stringLiteral_ = "";
  subVariableIdentifier_ = nullptr;
  functionCall_ = nullptr;
  valueManager_ = valueManager;
}

SubExpression::SubExpression(int sourceFileIndex, ValueManager* valueManager, FloatImmediate* floatValue)
  : TokenObject(sourceFileIndex) {
  type_ = SE_FLOAT;
  identifier_ = "";
  integerValue_ = -1;
  floatValue_ = floatValue->actualValue();
  stringLiteral_ = "";
  subVariableIdentifier_ = nullptr;
  functionCall_ = nullptr;
  delete floatValue;
  valueManager_ = valueManager;
}

SubExpression::SubExpression(int sourceFileIndex, ValueManager* valueManager, std::string stringLiteral, bool usingString)
  : TokenObject(sourceFileIndex) {
  type_ = SE_SL;
  identifier_ = "";
  integerValue_ = -1;
  floatValue_ = -1;
  stringLiteral_ = stringLiteral;
  subVariableIdentifier_ = nullptr;
  functionCall_ = nullptr;
  valueManager_ = valueManager;
}

SubExpression::SubExpression(int sourceFileIndex, ValueManager* valueManager, SubVariableIdentifier* subVariableIdentifier)
  : TokenObject(sourceFileIndex) {
  type_ = SE_SVI;
  identifier_ = "";
  integerValue_ = -1;
  floatValue_ = -1;
  stringLiteral_ = "";
  subVariableIdentifier_ = subVariableIdentifier;
  functionCall_ = nullptr;
  valueManager_ = valueManager;
}

SubExpression::SubExpression(int sourceFileIndex, ValueManager* valueManager, FunctionCall* functionCall)
  : TokenObject(sourceFileIndex) {
  type_ = SE_FC;
  identifier_ = "";
  integerValue_ = -1;
  floatValue_ = -1;
  stringLiteral_ = "";
  subVariableIdentifier_ = nullptr;
  functionCall_ = functionCall;
  valueManager_ = valueManager;
}

SubExpression::SubExpression(const SubExpression &original) 
 : TokenObject(original.sourceFileIndex_) {
  type_ = original.type_;
  valueManager_ = original.valueManager_;
  switch(type_) {
  case SE_ID:
    identifier_ = original.identifier_;
    subVariableIdentifier_ = nullptr;
    functionCall_ = nullptr;
    variableManager_ = original.variableManager_;
    sourceFile_ = original.sourceFile_;
    break;
  case SE_INT:
    integerValue_ = original.integerValue_;
    subVariableIdentifier_ = nullptr;
    functionCall_ = nullptr;
    break;
  case SE_FLOAT:
    floatValue_ = original.floatValue_;
    subVariableIdentifier_ = nullptr;
    functionCall_ = nullptr;
    break;
  case SE_SL:
    stringLiteral_ = original.stringLiteral_;
    subVariableIdentifier_ = nullptr;
    functionCall_ = nullptr;
    break;
  case SE_SVI:
    subVariableIdentifier_ = new SubVariableIdentifier(*(original.subVariableIdentifier_));
    functionCall_ = nullptr;
    break;
  case SE_FC:
    subVariableIdentifier_ = nullptr;
    functionCall_ = new FunctionCall(*(original.functionCall_));
    break;
  }
}

SubExpression::~SubExpression() {
  delete subVariableIdentifier_;
  delete functionCall_;
}

ReturnableValue* SubExpression::returningCode() {
  switch(type_) {
  case SE_ID: {
    Variable* variable = variableManager_->getVariable(identifier_);
    if(variable != nullptr) {
      return variable->value();
    } else {
      std::cout << "\033[1;41mERROR:\033[0m variable does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    break; }
  case SE_INT:
    //return new ReturnableValue(integerValue_);
    return valueManager_->newValue(integerValue_);
  case SE_FLOAT:
    //return new ReturnableValue(floatValue_);
    return valueManager_->newValue(floatValue_);
  case SE_SL:
    //return new ReturnableValue(stringLiteral_);
    return valueManager_->newValue(stringLiteral_);
  case SE_SVI: 
    return subVariableIdentifier_->returningCode();
  case SE_FC:
    return functionCall_->returningCode();
  }
}

ReturnableValue* SubExpression::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  switch(type_) {
  case SE_ID: {
    Variable* variable = classVariableManager->getVariable(identifier_);
    if(variable != nullptr) {
      return variable->value();
    } else {
      std::cout << "\033[1;41mERROR:\033[0m variable does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    break; }
  case SE_INT:
    //return new ReturnableValue(integerValue_);
    return valueManager_->newValue(integerValue_);
  case SE_FLOAT:
    //return new ReturnableValue(floatValue_);
    return valueManager_->newValue(floatValue_);
  case SE_SL:
    //return new ReturnableValue(stringLiteral_);
    return valueManager_->newValue(stringLiteral_);
  case SE_SVI:
    return subVariableIdentifier_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
  case SE_FC: {
    ReturnableValue* value = functionCall_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    return value; }
  }
}

bool SubExpression::immediateRun() {
  //this is only false for one particular function.
  if(type_ == SE_FC) {
    functionCall_->immediateRun();
  } else {
    return true;
  }
}

