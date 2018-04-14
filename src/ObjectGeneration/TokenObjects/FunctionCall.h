#ifndef MAJOR_FUNCTION_CALL_H
#define MAJOR_FUNCTION_CALL_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include "../ClassRunnable.h"
#include "../ValueReturnable.h"
#include "../ClassValueReturnable.h"
#include <string>

class ParameterSettingStatement;
class PreProcessedFile;
class EventManager;
class ClassDefinitionManager;
class ValueManager;

class FunctionCall : public TokenObject, CodeRunnable, ClassRunnable, ClassValueReturnable, ValueReturnable  {
private:
  std::string functionName_;
  
  //owned memory
  ParameterSettingStatement* parameterSettingStatement_;
  
  //memory not owned
  PreProcessedFile* sourceFile_;
  EventManager* eventManager_;
  ClassDefinitionManager* classDefinitionManager_;
  ValueManager* valueManager_;
public:
  //FUNCTION_CALL
  FunctionCall(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager_, ClassDefinitionManager* classDefinitionManager, EventManager* eventManager, std::string identifier, ParameterSettingStatement* parameterSettingStatement);
  //FUNCTION_CALL2
  FunctionCall(int sourceFileIndex, PreProcessedFile* sourceFile, std::string identifier);
  
  //copy constructor
  FunctionCall(const FunctionCall &original);

  ~FunctionCall();

  void runCode() override;
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  
  ReturnableValue* returningCode() override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  ReturnableValue* returningCodeSeparateClassMode(VariableManager* separateClassVariableManager,
                                                  VariableManager* classVariableManager,
                                                  MethodManager* classMethodManager,
                                                  CreatedClass* parentClass);
  ReturnableValue* returningCodeOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  
  
  std::string functionName();
  ParameterSettingStatement* parameterSettingStatement();
  
  bool immediateRun();
};

#endif
