#ifndef MAJOR_CLASS_CODE_BLOCK_H
#define MAJOR_CLASS_CODE_BLOCK_H

#include "TokenObject.h"
#include "../ClassRunnable.h"
#include <vector>

class PreProcessedFile;
class CodeBlock;
class MethodDefinition;
class ConstructionDefinition;

class ClassCodeBlock : public TokenObject, ClassRunnable {
private:
  //owned memory
  std::vector<CodeBlock*> allCodeBlocks_;
  std::vector<MethodDefinition*> allMethodDefinitions_;
  ConstructionDefinition* constructionDefinition_;
  
  //memory not owned
  PreProcessedFile* sourceFile_;
public: 
  //CLASS_CODE_BLOCK
  ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, CodeBlock* codeBlock);
  //CLASS_CODE_BLOCK2
  ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, CodeBlock* codeBlock, ClassCodeBlock* classCodeBlock);
  //CLASS_CODE_BLOCK3
  ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, MethodDefinition* methodDefinition);
  //CLASS_CODE_BLOCK4
  ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, MethodDefinition* methodDefinition, ClassCodeBlock* classCodeBlock);
  //CLASS_CODE_BLOCK5
  ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, ConstructionDefinition* constructionDefinition);
  //CLASS_CODE_BLOCK6
  ClassCodeBlock(int sourceFileIndex, PreProcessedFile* sourceFile, ConstructionDefinition* constructionDefinition, ClassCodeBlock* classCodeBlock);
  
  ~ClassCodeBlock();
  
  std::vector<CodeBlock*>* allCodeBlocks();
  std::vector<MethodDefinition*>* allMethodDefinitions();
  ConstructionDefinition* constructionDefinition();
  
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
