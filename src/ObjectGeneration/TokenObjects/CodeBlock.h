#ifndef MAJOR_CODE_BLOCK_H
#define MAJOR_CODE_BLOCK_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include "../ClassRunnable.h"
#include "../ClassValueReturnable.h"
#include <vector>

class Line;
class VariableManager;
class MethodManager;
class CreatedClass;
class ReturnableValue;


class CodeBlock : public TokenObject, CodeRunnable, ClassRunnable, ClassValueReturnable {
private:
  std::vector<Line*> allLines_;
public:
  //CODE_BLOCK
  CodeBlock(int sourceFileIndex, Line* line);
  //CODE_BLOCK2
  CodeBlock(int sourceFileIndex, Line* line, CodeBlock* codeBlock);
  
  //copy constructor
  CodeBlock(const CodeBlock &original);
  
  ~CodeBlock();
  
  std::vector<Line*>* allLines();
  
  void runCode() override;
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
