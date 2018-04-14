#ifndef MAJOR_VARIABLE_INITIATION_H
#define MAJOR_VARIABLE_INITIATION_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include "../ClassRunnable.h"
#include <string>

class VariableManager;
class PreProcessedFile;

class VariableInitiation : public TokenObject, CodeRunnable, ClassRunnable {
private:
  //not owned memory
  VariableManager* variableManager_;
  PreProcessedFile* sourceFile_;
  
  //static memory
  std::string identifier1_;
  std::string identifier2_;
public:
  //VARIABLE_INITIATION
  VariableInitiation(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, std::string identifier1, std::string identifier2);
  
  //copy constructor
  VariableInitiation(const VariableInitiation &original);
  
  ~VariableInitiation();
  
  std::string identifier1();
  std::string identifier2();
  
  void runCode() override;
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
