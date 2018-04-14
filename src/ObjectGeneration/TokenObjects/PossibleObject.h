#ifndef MAJOR_POSSIBLE_OBJECT_H
#define MAJOR_POSSIBLE_OBJECT_H

#include "TokenObject.h"
#include <string>
#include "../ValueReturnable.h"
#include "../ClassValueReturnable.h"

class FunctionCall;
class VariableManager;
class PreProcessedFile;
class Variable;

typedef enum {
  PO_I,
  PO_FC
} PossibleObjectType;

class PossibleObject : public TokenObject, ValueReturnable, ClassValueReturnable {
private:

  PossibleObjectType type_;

  std::string identifier_;
  
  //owned memory
  FunctionCall* functionCall_;
  
  //memory not owned
  VariableManager* variableManager_;
  PreProcessedFile* sourceFile_;
public:
  //POSSIBLE_OBJECT
  PossibleObject(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, std::string identifier);
  //POSSIBLE_OBJECT2
  PossibleObject(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, FunctionCall* functionCall);
  
  //copy constructor
  PossibleObject(const PossibleObject &original);
  
  ~PossibleObject();
  
  ReturnableValue* returningCode() override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  ReturnableValue* returningCodeSeparateClassMode(VariableManager* separateClassVariableManager,
                                                  VariableManager* classVariableManager,
                                                  MethodManager* classMethodManager,
                                                  CreatedClass* parentClass);
  ReturnableValue* returningCodeOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  
  Variable* getVariable(VariableManager* classVariableManager);
  
};

#endif
