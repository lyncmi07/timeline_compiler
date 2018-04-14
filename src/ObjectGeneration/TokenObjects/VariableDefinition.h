#ifndef MAJOR_VARIABLE_DEFINITION_H
#define MAJOR_VARIABLE_DEFINITION_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include "../ClassRunnable.h"
#include <string>

class VariableInitiation;
class Expression;
class SubVariableIdentifier;
class VariableManager;
class PreProcessedFile;

typedef enum {
  VD_VI,
  VD_I,
  VD_SVI
} VariableDefinitionType;

class VariableDefinition : public TokenObject, CodeRunnable, ClassRunnable {
private:
  VariableDefinitionType type_;
  
  //owned memory
  VariableInitiation* variableInitiation_;
  std::string identifier_;
  SubVariableIdentifier* subVariableIdentifier_;
  Expression* expression_;
  
  //memory not owned
  VariableManager* variableManager_;
  PreProcessedFile* sourceFile_;
  
public:
  //VARIABLE_DEFINITION
  VariableDefinition(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, VariableInitiation* variableInitiation, Expression* expression);
  //VARIABLE_DEFINITION2
  VariableDefinition(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, std::string identifier, Expression* expression);
  //VARIABLE_DEFINITION3
  VariableDefinition(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, SubVariableIdentifier* subVariableIdentifier, Expression* expression);
  
  //copy constructor
  VariableDefinition(const VariableDefinition &original);
  
~VariableDefinition();

  void runCode() override;
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
