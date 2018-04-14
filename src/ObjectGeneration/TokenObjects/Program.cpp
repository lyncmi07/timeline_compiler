#include "Program.h"
#include "CodeBlock.h"
#include "ClassDefinition.h"
#include <iostream>
Program::Program(int sourceFileIndex, CodeBlock* codeBlock) 
  : TokenObject(sourceFileIndex) {
  allCodeBlocks_.push_back(codeBlock);
}

Program::Program(int sourceFileIndex, CodeBlock* codeBlock, Program* program) 
  : TokenObject(sourceFileIndex) {
  allCodeBlocks_.push_back(codeBlock);
  for(int i = 0; i < program->allCodeBlocks()->size(); i++) {
    allCodeBlocks_.push_back(program->allCodeBlocks()->at(i));
  }
  for(int i = 0; i < program->allClassDefinitions()->size(); i++) {
    allClassDefinitions_.push_back(program->allClassDefinitions()->at(i));
  }
  
  delete program;
}

Program::Program(int sourceFileIndex, ClassDefinition* classDefinition)
  : TokenObject(sourceFileIndex) {
  allClassDefinitions_.push_back(classDefinition);
}

Program::Program(int sourceFileIndex, ClassDefinition* classDefinition, Program* program)
  : TokenObject(sourceFileIndex) {
  allClassDefinitions_.push_back(classDefinition);
  for(int i = 0; i < program->allCodeBlocks()->size(); i++) {
    allCodeBlocks_.push_back(program->allCodeBlocks()->at(i));
  }
  for(int i = 0; i < program->allClassDefinitions()->size(); i++) {
    allClassDefinitions_.push_back(program->allClassDefinitions()->at(i));
  }
  
  delete program;
}

std::vector<CodeBlock*>* Program::allCodeBlocks() {
  return &allCodeBlocks_;
}

std::vector<ClassDefinition*>* Program::allClassDefinitions() {
  return &allClassDefinitions_;
}

void Program::runCode() {
  for(int i = 0; i < allClassDefinitions_.size(); i++) {
    allClassDefinitions_[i]->runCode();
  }


  for(int i = 0; i < allCodeBlocks_.size(); i++) {
    allCodeBlocks_[i]->runCode();
  }
}
