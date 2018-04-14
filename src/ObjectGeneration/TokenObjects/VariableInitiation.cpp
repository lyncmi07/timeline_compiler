#include "VariableInitiation.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Helpers/StringFunctions.h"
#include <iostream>
#include "../../Variables/VariableManager.h"

VariableInitiation::VariableInitiation(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, std::string identifier1, std::string identifier2)
  : TokenObject(sourceFileIndex) {
  variableManager_ = variableManager;
  identifier1_ = identifier1;
  identifier2_ = identifier2;
  sourceFile_ = sourceFile;
  identifier1_ = str_func::removeWhitespace(identifier1_);
  identifier2_ = str_func::removeWhitespace(identifier2_);
}

VariableInitiation::VariableInitiation(const VariableInitiation &original) 
 : TokenObject(original.sourceFileIndex_) {
  variableManager_ = original.variableManager_;
  sourceFile_ = original.sourceFile_;
  
  identifier1_ = original.identifier1_;
  identifier2_ = original.identifier2_;
}

VariableInitiation::~VariableInitiation() {

}

std::string VariableInitiation::identifier1() {
  return identifier1_;
}

std::string VariableInitiation::identifier2() {
  return identifier2_;
}

void VariableInitiation::runCode() {
  if(variableManager_->addVariable(identifier1_, identifier2_) == nullptr) {
    //throw a variable already defined error
    std::cout << "\033[1;41mERROR:\033[0m Variable '" << identifier2_ << "' already defined: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
  }
}

void VariableInitiation::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  if(classVariableManager->addVariable(identifier1_, identifier2_) == nullptr) {
    //throw a variable already defined error
    std::cout << "\033[1;41mERROR:\033[0m Variable '" << identifier2_ << "' already defined: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
  }
}
