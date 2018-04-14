#include "ClassCodeBlock.h"
#include <iostream>
#include "../../Parser/PreProcessedFile.h"
#include "CodeBlock.h"
#include "../../Variables/MethodManager.h"
#include "../../Exceptions/CompilerExceptions.h"

ClassCodeBlock::ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, CodeBlock* codeBlock)
  : TokenObject(sourceFileIndex) {
  allCodeBlocks_.push_back(codeBlock);
  constructionDefinition_ = nullptr;
  sourceFile_ = sourceFile;
}

ClassCodeBlock::ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, CodeBlock* codeBlock, ClassCodeBlock* classCodeBlock)
  : TokenObject(sourceFileIndex) {
  allCodeBlocks_.push_back(codeBlock);
  for(int i = 0; i < classCodeBlock->allCodeBlocks()->size(); i++) {
    allCodeBlocks_.push_back(classCodeBlock->allCodeBlocks()->at(i));
  }
  //ensure that CodeBlock objects are not destroyed in classCodeBlock deletion
  classCodeBlock->allCodeBlocks()->clear();
  
  for(int i = 0; i < classCodeBlock->allMethodDefinitions()->size(); i++) {
    allMethodDefinitions_.push_back(classCodeBlock->allMethodDefinitions()->at(i));
  }
  //ensure that MethodDefinition objects are not destroyed in classCodeBlock deletion
  classCodeBlock->allMethodDefinitions()->clear();
  
  constructionDefinition_ = classCodeBlock->constructionDefinition();
  classCodeBlock->constructionDefinition_ = nullptr;
  
  sourceFile_ = sourceFile;
  
  delete classCodeBlock;
}

ClassCodeBlock::ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, MethodDefinition* methodDefinition) 
  : TokenObject(sourceFileIndex) {
  allMethodDefinitions_.push_back(methodDefinition);
  constructionDefinition_ = nullptr;
  sourceFile_ = sourceFile;
}

ClassCodeBlock::ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, MethodDefinition* methodDefinition, ClassCodeBlock* classCodeBlock) 
  : TokenObject(sourceFileIndex) {
  
  allMethodDefinitions_.push_back(methodDefinition);
  
  for(int i = 0; i < classCodeBlock->allCodeBlocks()->size(); i++) {
    allCodeBlocks_.push_back(classCodeBlock->allCodeBlocks()->at(i));
  }
  //ensure that CodeBlock objects are not destroyed in classCodeBlock deletion
  classCodeBlock->allCodeBlocks()->clear();
  
  for(int i = 0; i < classCodeBlock->allMethodDefinitions()->size(); i++) {
    allMethodDefinitions_.push_back(classCodeBlock->allMethodDefinitions()->at(i));
  }
  //ensure that MethodDefinition objects are not destroyed in classCodeBlock deletion
  classCodeBlock->allMethodDefinitions()->clear();
  
  constructionDefinition_ = classCodeBlock->constructionDefinition();
  classCodeBlock->constructionDefinition_ = nullptr;
  
  sourceFile_ = sourceFile;
  
  delete classCodeBlock;
}

ClassCodeBlock::ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, ConstructionDefinition* constructionDefinition)
  : TokenObject(sourceFileIndex) {
  constructionDefinition_ = constructionDefinition;
  sourceFile_ = sourceFile;
}

ClassCodeBlock::ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, ConstructionDefinition* constructionDefinition, ClassCodeBlock* classCodeBlock) 
  : TokenObject(sourceFileIndex) {
  if(classCodeBlock->constructionDefinition() != nullptr) {
    std::cout << "\033[1;41mERROR:\033[0m Class can only have one Constructor: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::StructureException();
  } else {
    constructionDefinition_ = constructionDefinition;
    
    for(int i = 0; i < classCodeBlock->allCodeBlocks()->size(); i++) {
      allCodeBlocks_.push_back(classCodeBlock->allCodeBlocks()->at(i));
    }
    //ensure that CodeBlock objects are not destroyed in classCodeBlock deletion
    classCodeBlock->allCodeBlocks()->clear();
  
    for(int i = 0; i < classCodeBlock->allMethodDefinitions()->size(); i++) {
      allMethodDefinitions_.push_back(classCodeBlock->allMethodDefinitions()->at(i));
    }
    
    sourceFile_ = sourceFile;
    
    //ensure that MethodDefinition objects are not destroyed in classCodeBlock deletion
    classCodeBlock->allMethodDefinitions()->clear();
  }
  
  delete classCodeBlock;
}

ClassCodeBlock::~ClassCodeBlock() {
  for(int i = 0; allCodeBlocks_.size(); i++) {
    delete allCodeBlocks_[i];
  }
  for(int i = 0; allMethodDefinitions_.size(); i++) {
    delete allMethodDefinitions_[i];
  }
  delete constructionDefinition_;
}

std::vector<CodeBlock*>* ClassCodeBlock::allCodeBlocks() {
  return &allCodeBlocks_;
}

std::vector<MethodDefinition*>* ClassCodeBlock::allMethodDefinitions() {
  return &allMethodDefinitions_;
}

ConstructionDefinition* ClassCodeBlock::constructionDefinition() {
  return constructionDefinition_;
}

void ClassCodeBlock::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {

  //put variables into class variable manager
  for(int i = 0; i < allCodeBlocks_.size(); i++) {
    allCodeBlocks_[i]->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
  }
  
  //put methods into class method manager
  for(int i = 0; i < allMethodDefinitions_.size(); i++) {
    classMethodManager->addMethod(allMethodDefinitions_[i]);
  }
}
