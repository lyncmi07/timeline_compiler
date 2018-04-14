#ifndef MAJOR_PROGRAM_H
#define MAJOR_PROGRAM_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include <vector>

class CodeBlock;
class ClassDefinition;

class Program : public TokenObject, CodeRunnable {
private:
  std::vector<CodeBlock*> allCodeBlocks_;
  std::vector<ClassDefinition*> allClassDefinitions_;
public:
  //constructor for each rule of the token

  //PROGRAM
  Program(int sourceFileIndex, CodeBlock* codeBlock);
  //PROGRAM2
  Program(int sourceFileIndex, CodeBlock* codeBlock, Program* program); 
  //PROGRAM3
  Program(int sourceFileIndex, ClassDefinition* classDefinition);
  //PROGRAM4
  Program(int sourceFileIndex, ClassDefinition* classDefinition, Program* program);
  
  std::vector<CodeBlock*>* allCodeBlocks();
  std::vector<ClassDefinition*>* allClassDefinitions();
  
  void runCode() override;
};

#endif
