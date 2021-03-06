#include "VariableManager.h"
#include "../Helpers/StringFunctions.h"
#include <iostream>
#include "Variable.h"

VariableManager::VariableManager() {
  localVariableManager_ = nullptr;
}

VariableManager::VariableManager(const VariableManager &original) {
  localVariableManager_ = nullptr;
  for(int i = 0; i < original.localManagers_.size(); i++) {
    localManagers_.push_back(original.localManagers_[i]);
    localManagerStack_.push_back(original.localManagerStack_[i]);
  }
  
  for(int i = 0; i < original.allVariables_.size(); i++) {
    addVariable(original.allVariables_[i]->variableType(), original.allVariables_[i]->identifier());
  }
  localVariableManager_ = original.localVariableManager_;
}

VariableManager::~VariableManager() {
  for(int i = 0; i < allVariables_.size(); i++) {
    delete allVariables_[i];
  }
  for(int i = 0; i < localManagers_.size(); i++) {
    delete localManagers_[i];
  }
}

bool VariableManager::checkVariableExistence(std::string identifier) {
  for(int i = 0; i < allVariables_.size(); i++) {
    if(allVariables_[i]->identifier().compare(identifier) == 0) {
      return true;
    }
  }
  
  return false;
}

std::vector<Variable*>* VariableManager::allVariables() {
  return &allVariables_;
}

Variable* VariableManager::getVariable(std::string identifier) {
  identifier = str_func::removeWhitespace(identifier);
  for(int i = 0; i < allVariables_.size(); i++) {
    if(allVariables_[i]->identifier().compare(identifier) == 0) {
      
      return allVariables_[i];
    }
  }
  if(localVariableManager_ != nullptr) {  
    return localVariableManager_->getVariable(identifier);
  }
  return nullptr;
}

Variable* VariableManager::addVariable(std::string variableType, std::string identifier) {
  if(checkVariableExistence(identifier)) {
    return nullptr;
  }
  Variable* newVar = new Variable(variableType, str_func::removeWhitespace(identifier));
  
  if(localVariableManager_ != nullptr) {
    localVariableManager_->allVariables()->push_back(newVar);
  } else {
    allVariables_.push_back(newVar);
  }
  
  return newVar;
}

Variable* VariableManager::addVariable(std::string variableType, std::string identifier, Expression* value) {
  Variable* newVar = addVariable(variableType, identifier);
  if(newVar == nullptr) {
    return nullptr;
  }
  
  newVar->setValue(value);
  return newVar;
}

void VariableManager::addLocalVariableManager(VariableManager* localVariableManager) {
  localManagers_.push_back(localVariableManager);
  localManagerStack_.push_back(localManagers_.size() - 1);
  localVariableManager_ = localVariableManager;
}

void VariableManager::removeLocalVariableManager() {
  localManagerStack_.pop_back();
  if(localManagerStack_.size() > 0) {
    localVariableManager_ = localManagers_[localManagerStack_[localManagerStack_.size()-1]];
  } else {
    localVariableManager_ = nullptr;
  }
}

VariableManager* VariableManager::localVariableManager() {
  return localVariableManager_;
}

void VariableManager::listGeneratedVariables() {
  std::cout << "Listing variables:" << std::endl;
  for(int i = 0; i < allVariables_.size(); i++) {
    std::cout << allVariables_[i]->toString() << std::endl;
  }
}

std::string VariableManager::printToXML(int spaceIndent) {
  std::string returnString = "";
  std::string indent(spaceIndent,' ');
  
  returnString += indent + "<variables>\n";
  for(int i = 0; i < allVariables_.size(); i++) {
    returnString += allVariables_[i]->printToXML(spaceIndent+2);
  }
  returnString += indent + "</variables>\n";
  
  return returnString;
}
