#include "CreatedClass.h"
#include <iostream>
CreatedClass::CreatedClass(std::string className, VariableManager* subVariables, MethodManager* methods, ConstructionDefinition* constructionDefinition, CreatedClass* parentClass) {
  className_ = className;
  constructionDefinition_ = constructionDefinition;
  variableManager_ = subVariables;
  methodManager_ = methods;
  constructionDefinition_ = constructionDefinition;
  parentClass_ = parentClass;
}

CreatedClass::~CreatedClass() {
  delete constructionDefinition_;
  delete variableManager_;
  delete methodManager_;
}

ConstructionDefinition* CreatedClass::constructionDefinition() {
  return constructionDefinition_;
}

std::string CreatedClass::className() {
  return className_;
}

VariableManager* CreatedClass::subVariables() {
  return variableManager_;
}

MethodManager* CreatedClass::methods() {
  return methodManager_;
}

CreatedClass* CreatedClass::parentClass() {
  return parentClass_;
}

CreatedClass* CreatedClass::baseClass() {
  if(parentClass_ != nullptr) {
    return parentClass_->baseClass();
  } else {
    return this;
  }
}
