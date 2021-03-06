#ifndef MAJOR_CONSTRUCTION_DEFINITION_H
#define MAJOR_CONSTRUCTION_DEFINITION_H

#include "TokenObject.h"

class ParameterDefinitionStatement;
class ParameterSettingStatement;
class CodeBlock;
class VariableManager;
class MethodManager;
class CreatedClass;
class PreProcessedFile;

class ConstructionDefinition : public TokenObject {
private:
  //owned memory
  ParameterDefinitionStatement* parameterDefinitionStatement_;
  CodeBlock* codeBlock_;

  //memory not owned
  PreProcessedFile* sourceFile_;

  void convertParametersToVariables(ParameterSettingStatement* parameterInputs, VariableManager* localVariableManager);
  void convertParametersToVariablesSuperMode(ParameterSettingStatement* parameterInputs, VariableManager* localVariableManager, VariableManager* classVariableManager, MethodManager* classMethodManager);

public:
  //CONSTRUCTION_DEFINITION
  ConstructionDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, ParameterDefinitionStatement* parameterDefinitionStatement, CodeBlock* codeBlock);
  //CONSTRUCTION_DEFINITION2
  ConstructionDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, ParameterDefinitionStatement* parameterDefinitionStatement);
  
  ~ConstructionDefinition();
  
  void runAsConstructionCode(ParameterSettingStatement* constructionInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  void runAsSuperCode(ParameterSettingStatement* constructionInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
};

#endif
