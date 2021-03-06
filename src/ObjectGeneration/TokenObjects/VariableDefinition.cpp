#include "VariableDefinition.h"
#include "../../Parser/PreProcessedFile.h"
#include <iostream>
#include "VariableInitiation.h"
#include "../../Variables/VariableManager.h"
#include "../../Variables/Variable.h"
#include "SubVariableIdentifier.h"
#include "Expression.h"
#include "../../Helpers/StringFunctions.h"

VariableDefinition::VariableDefinition(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile,  VariableInitiation* variableInitiation, Expression* expression)
  : TokenObject(sourceFileIndex) {
  type_ = VD_VI;
  variableInitiation_ = variableInitiation;
  subVariableIdentifier_ = nullptr;
  expression_ = expression;
  variableManager_ = variableManager;
  sourceFile_ = sourceFile;
}

VariableDefinition::VariableDefinition(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, std::string identifier, Expression* expression)
  : TokenObject(sourceFileIndex) {
  type_ = VD_I;
  variableInitiation_ = nullptr;
  subVariableIdentifier_ = nullptr;
  identifier_ = str_func::removeWhitespace(identifier);
  expression_ = expression;
  variableManager_ = variableManager;
  sourceFile_ = sourceFile;
}

VariableDefinition::VariableDefinition(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, SubVariableIdentifier* subVariableIdentifier, Expression* expression)
  : TokenObject(sourceFileIndex) {
  type_ = VD_SVI;
  variableInitiation_ = nullptr;
  subVariableIdentifier_ = subVariableIdentifier;
  expression_ = expression;
  variableManager_ = variableManager;
  sourceFile_ = sourceFile;
}

VariableDefinition::VariableDefinition(const VariableDefinition &original) 
 : TokenObject(original.sourceFileIndex_) {
  type_ = original.type_;
  variableManager_ = original.variableManager_;
  sourceFile_ = original.sourceFile_;  
  
  switch(type_) {
  case VD_VI:
    variableInitiation_ = new VariableInitiation(*(original.variableInitiation_));
    subVariableIdentifier_ = nullptr;
    expression_ = new Expression(*(original.expression_));
    break;
  case VD_I:
    variableInitiation_ = nullptr;
    subVariableIdentifier_ = nullptr;
    identifier_ = original.identifier_;
    expression_ = new Expression(*(original.expression_));
    break;
  case VD_SVI:
    variableInitiation_ = nullptr;
    subVariableIdentifier_ = new SubVariableIdentifier(*(original.subVariableIdentifier_));
    expression_ = new Expression(*(original.expression_));
  }
}

VariableDefinition::~VariableDefinition() {
  delete variableInitiation_;
  delete subVariableIdentifier_;
  delete expression_;
}

void VariableDefinition::runCode() {
  switch(type_) {
  case VD_VI: {
    variableInitiation_->runCode();
    Variable* variable = variableManager_->getVariable(variableInitiation_->identifier2());
    if(variable != nullptr) {
      variable->setValue(expression_);    
    } else {
      std::cout << "\033[1;41mERROR:\033[0m Variable '" << variableInitiation_->identifier2() << "' does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    }
    break; }
  case VD_I: {
    Variable* variable = variableManager_->getVariable(identifier_);
    if(variable != nullptr) {
      variable->setValue(expression_);
    } else {
      std::cout << "\033[1;41mERROR:\033[0m Variable '" << identifier_ << "' does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    }
    break; }
  case VD_SVI: {
    Variable* variable = subVariableIdentifier_->getVariable();
    
    if(variable != nullptr) {
      variable->setValue(expression_);
    } else {
      std::cout << "\033[1;41mERROR:\033[0m Variable '" << identifier_ << "' does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    }
    
    //std::cout << "ERROR: not yet implemented: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    break; }
  }
}

void VariableDefinition::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  switch(type_) {
  case VD_VI: {
    variableInitiation_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
    Variable* variable = classVariableManager->getVariable(variableInitiation_->identifier2());
    if(variable != nullptr) {
      variable->setValueInsideClass(expression_, classVariableManager, classMethodManager, parentClass);
    } else {
      std::cout << "\033[1;41mERROR:\033[0m Variable '" << identifier_ << "' does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    }
    break; }
  case VD_I: {
    Variable* variable = classVariableManager->getVariable(identifier_);
    if(variable != nullptr) {
      variable->setValueInsideClass(expression_, classVariableManager, classMethodManager, parentClass);
    } else {
      std::cout << "\033[1;41mERROR:\033[0m Variable '" << identifier_ << "' does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    }
    break; }
  case VD_SVI: {
    //std::cout << "ERROR: not yet implemented: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    Variable* variable = subVariableIdentifier_->getVariableClassMode(classVariableManager, classMethodManager, parentClass);
    
    if(variable != nullptr) {
      variable->setValueInsideClass(expression_, classVariableManager, classMethodManager, parentClass);
    } else {
      std::cout << "\033[1;41mERROR:\033[0m Variable '" << identifier_ << "' does not exist: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    } 
    break; }
  }
}




