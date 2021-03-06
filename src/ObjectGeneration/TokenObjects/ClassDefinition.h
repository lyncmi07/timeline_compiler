#ifndef MAJOR_CLASS_DEFINITION_H
#define MAJOR_CLASS_DEFINITION_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include <string>

class ClassCodeBlock;
class ClassDefinitionManager;

class ClassDefinition : public TokenObject, CodeRunnable {
private:
  std::string className_;
  std::string parentClassName_;
  
  //owned memory
  ClassCodeBlock* classCodeBlock_;
  
  //memory not owned
  ClassDefinitionManager* classDefinitionManager_;
    
public:
  //CLASS_DEFINITION
  ClassDefinition(int sourceFileIndex, ClassDefinitionManager* classDefinitionManager, std::string identifier1, std::string identifier2, ClassCodeBlock* classCodeBlock);
  //CLASS_DEFINITION2
  ClassDefinition(int sourceFileIndex, ClassDefinitionManager* classDefinitionManager, std::string identifier1, std::string identifier2);
  
  ~ClassDefinition();
  
  std::string className();
  std::string parentClassName();
  ClassCodeBlock* classCodeBlock();
  
  void runCode() override;
};

#endif
