#ifndef MAJOR_METHOD_DEFINITION_H
#define MAJOR_METHOD_DEFINITION_H

#include "TokenObject.h"
#include <string>

class ParameterDefinitionStatement;
class CodeBlock;
class ParameterSettingStatement;
class VariableManager;
class MethodManager;
class CreatedClass;
class ReturnableValue;
class PreProcessedFile;


class MethodDefinition : public TokenObject {
private:
  std::string returnType_;
  std::string methodIdentifier_;
  
  //owned memory
  ParameterDefinitionStatement* parameterDefinitionStatement_;
  CodeBlock* codeBlock_;
  
  PreProcessedFile* sourceFile_;
  
  void convertParametersToVariables(ParameterSettingStatement* parameterInputs, VariableManager* localVariableManager);
  void convertParametersToVariablesInsideClass(ParameterSettingStatement* parameterInputs, VariableManager* localVariableManager, VariableManager* classVariableManager, MethodManager* classMethodManager);
public:
  //METHOD_DEFINITION
  MethodDefinition(int sourceFileIndex, PreProcessedFile* sourceFile_, std::string identifier1, std::string identifier2, ParameterDefinitionStatement* parameterDefinitionStatement, CodeBlock* codeBlock);
  //METHOD_DEFINITION2
  MethodDefinition(int sourceFileIndex, PreProcessedFile* sourceFile_, std::string identifier1, std::string identifier2, ParameterDefinitionStatement* parameterDefinitionStatement);
  
  MethodDefinition(const MethodDefinition &original);
  
  ~MethodDefinition();
  
  std::string returnType();
  std::string methodIdentifier();
  
  ParameterDefinitionStatement* parameterDefinitionStatement();
  CodeBlock* codeBlock();
  
  void runAsVoidMethod(ParameterSettingStatement* methodInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  ReturnableValue* runAsReturningMethod(ParameterSettingStatement* methodInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  ReturnableValue* runAsReturningMethodInSeparateClass(ParameterSettingStatement* methodInputs,
                                                       VariableManager* separateClassVariableManager,
                                                       VariableManager* classVariableManager,
                                                       MethodManager* classMethodManager,
                                                       CreatedClass* parentClass);
  void runAsVoidMethodInsideClass(ParameterSettingStatement* methodInputs, VariableManager* classVarableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  ReturnableValue* runAsReturningMethodInsideClass(ParameterSettingStatement* methodInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
};

#endif
