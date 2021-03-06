#include "MethodManager.h"
#include <iostream>
#include "../ObjectGeneration/TokenObjects/MethodDefinition.h"

MethodManager::MethodManager() {

}

MethodManager::MethodManager(const MethodManager &original) {
  for(int i = 0; i < original.allMethods_.size(); i++) {
    allMethods_.push_back(new MethodDefinition(*(original.allMethods_[i])));
  }
}

MethodManager::~MethodManager() {
  for(int i = 0; i < allMethods_.size(); i++) {
    delete allMethods_[i];
  }
}

MethodDefinition* MethodManager::getMethod(std::string identifier) {
  for(int i = 0; i < allMethods_.size(); i++) {
    if(allMethods_[i]->methodIdentifier().compare(identifier) == 0) {
      return allMethods_[i];
    }
  }
  
  return nullptr;
}

void MethodManager::addMethod(MethodDefinition* newMethod) {
  bool isOverride = false;
  for(int i = 0; i < allMethods_.size(); i++) {
    if(allMethods_[i]->methodIdentifier().compare(newMethod->methodIdentifier()) == 0) {
      //override the previous method
      delete allMethods_[i];
      allMethods_[i] = newMethod;
      isOverride = true;
    }
  }
  if(!isOverride) {
    allMethods_.push_back(newMethod);
  }
}

std::vector<MethodDefinition*>* MethodManager::allMethods() {
  return &allMethods_;
}

void MethodManager::listMethods() {
  for(int i = 0; i < allMethods_.size(); i++) {
    std::cout << allMethods_[i]->returnType() << " " << allMethods_[i]->methodIdentifier() << "()" << std::endl;
  }
}
