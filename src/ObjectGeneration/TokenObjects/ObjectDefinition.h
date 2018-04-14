#ifndef MAJOR_OBJECT_DEFINITION_H
#define MAJOR_OBJECT_DEFINITION_H

#include "TokenObject.h"
#include "../ValueReturnable.h"
#include "../ClassValueReturnable.h"

class FunctionCall;
class ArrayImmediateDefinition;
class ReturnableValue;
class PreProcessedFile;
class EventManager;
class EntityManager;
class ClassDefinitionManager;
class ValueManager;

class ObjectDefinition : public TokenObject, ValueReturnable, ClassValueReturnable {
private:
  //owned memory
  FunctionCall* functionCall_;
  ArrayImmediateDefinition* arrayImmediateDefinition_;
  
  //memory not owned
  PreProcessedFile* sourceFile_;
  ClassDefinitionManager* classDefinitionManager_;
  ValueManager* valueManager_;
  EventManager* eventManager_;
  EntityManager* entityManager_;
public:
  //OBJECT_DEFINITION
  ObjectDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, ClassDefinitionManager* classDefinitionManager, ValueManager* valueManager, EventManager* eventManager, EntityManager* entityManager, FunctionCall* functionCall, ArrayImmediateDefinition* arrayImmediateDefinition);
  
  //copy constructor
  ObjectDefinition(const ObjectDefinition &original);
  
  ~ObjectDefinition();
  
  ReturnableValue* returningCode() override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
