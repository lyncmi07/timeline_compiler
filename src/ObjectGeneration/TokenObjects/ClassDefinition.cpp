#include "ClassDefinition.h"

#include <iostream>

#include "../../Variables/ClassDefinitionManager.h"

ClassDefinition::ClassDefinition(int sourceFileIndex, ClassDefinitionManager* classDefinitionManager, std::string identifier1, std::string identifier2, ClassCodeBlock* classCodeBlock)
  : TokenObject(sourceFileIndex) {
  className_ = identifier1;
  parentClassName_ = identifier2;
  classCodeBlock_ = classCodeBlock;
  classDefinitionManager_ = classDefinitionManager;
}

ClassDefinition::ClassDefinition(int sourceFileIndex, ClassDefinitionManager* classDefinitionManager, std::string identifier1, std::string identifier2)
  : TokenObject(sourceFileIndex) {
 className_ = identifier1;
 parentClassName_ = identifier2;
 classCodeBlock_ = nullptr;
 classDefinitionManager_ = classDefinitionManager;
}

ClassDefinition::~ClassDefinition() {
  delete classCodeBlock_;
}

std::string ClassDefinition::className() {
  return className_;
}

std::string ClassDefinition::parentClassName() {
  return parentClassName_;
}

ClassCodeBlock* ClassDefinition::classCodeBlock() {
  return classCodeBlock_;
}

void ClassDefinition::runCode() {
  classDefinitionManager_->addNewClassDefinition(this);
}
