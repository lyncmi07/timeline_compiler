#include "CodeBlock.h"
#include "Line.h"
#include <iostream>

CodeBlock::CodeBlock(int sourceFileIndex, Line* line) 
  : TokenObject(sourceFileIndex) {
  allLines_.push_back(line);
}

CodeBlock::CodeBlock(int sourceFileIndex, Line* line, CodeBlock* codeBlock)
  : TokenObject(sourceFileIndex) {
  allLines_.push_back(line);
  for(int i = 0; i < codeBlock->allLines()->size(); i++) {
    allLines_.push_back(codeBlock->allLines()->at(i));
  }
  //ensure that the lines do not get destroyed in codeBlock destructor
  codeBlock->allLines()->clear();
  
  delete codeBlock;
}

CodeBlock::CodeBlock(const CodeBlock &original)
 : TokenObject(original.sourceFileIndex_) {
  for(int i = 0; i < original.allLines_.size(); i++) {
    allLines_.push_back(new Line(*(original.allLines_[i])));
  }
}

CodeBlock::~CodeBlock() {
  for(int i = 0; i < allLines_.size(); i++) {
    delete allLines_[i];
  }
}

//TODO: write a destructor that deletes all lines but doesn't delete them if the lines are being used by a parent codeBlock

std::vector<Line*>* CodeBlock::allLines() {
  return &allLines_;
}

void CodeBlock::runCode() {
  for(int i = 0; i < allLines_.size(); i++) {

    allLines_[i]->runCode();

  }
}

void CodeBlock::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  for(int i = 0; i < allLines_.size(); i++) {
    allLines_[i]->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
  }
}

ReturnableValue* CodeBlock::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  for(int i = 0; i < allLines_.size(); i++) {
    ReturnableValue* value = allLines_[i]->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    if(value != nullptr) {
      return value;
    }
  }
  
  return nullptr;
}
