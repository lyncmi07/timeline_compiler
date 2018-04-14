#ifndef MAJOR_SUB_VARIABLE_IDENTIFIER_H
#define MAJOR_SUB_VARIABLE_IDENTIFIER_H

#include "TokenObject.h"
#include <vector>
#include <iostream>

#include "../CodeRunnable.h"
#include "../ValueReturnable.h"
#include "../ClassRunnable.h"
#include "../ClassValueReturnable.h"
#include "../../Variables/ReturnableValue.h"
#include "../../Variables/Object.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Variables/CreatedClass.h"

class PossibleObject;
class ReturnableValue;
class PreProcessedFile;
class ValueManager;
class ClassDefinitionManager;

typedef enum {
  SVI_PP,
  SVI_PS
} SubVariableIdentifierType;

class SubVariableIdentifier : public TokenObject, CodeRunnable, ValueReturnable, ClassRunnable, ClassValueReturnable {

  SubVariableIdentifierType type_;

  //owned memory
  PossibleObject* possibleObject1_;
  PossibleObject* possibleObject2_;
  PossibleObject* possibleObject_;
  SubVariableIdentifier* subVariableIdentifier_;
  
  //memory not owned
  PreProcessedFile* sourceFile_;
  ValueManager* valueManager_;
  
public:
  //SUB_VARIABLE_IDENTIFIER
  SubVariableIdentifier(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, PossibleObject* possibleObject1, PossibleObject* possibleObject2);
  //SUB_VARIABLE_IDENTIFIER2
  SubVariableIdentifier(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, PossibleObject* possibleObject, SubVariableIdentifier* subVariableIdentifier);
  
  //copy constructor
  SubVariableIdentifier(const SubVariableIdentifier &original);
  
  ~SubVariableIdentifier();
  
  SubVariableIdentifierType type();
  
  void runCode() override;
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  ReturnableValue* returningCode() override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  //possibly make private
  ReturnableValue* returningCodeSeparateClassMode(VariableManager* separateClassVariableManager, 
                                                  VariableManager* classVariableManager, 
                                                  MethodManager* classMethodManager, 
                                                  CreatedClass* parentClass);
  ReturnableValue* returningCodeOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  
  Variable* getVariable();
  Variable* getVariableClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  //possibly make private
  Variable* getVariableOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  Variable* getVariableSeparateClassMode(VariableManager* separateClassVariableManager, 
                                         VariableManager* classVariableManager,
                                         MethodManager* classMethodManager,
                                         CreatedClass* parentClass);
};

#endif
