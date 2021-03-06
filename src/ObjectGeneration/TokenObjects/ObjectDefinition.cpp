#include "ObjectDefinition.h"
#include <iostream>

#include "FunctionCall.h"
#include "ArrayImmediateDefinition.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Variables/ClassDefinitionManager.h"
#include "../../Variables/EventManager.h"
#include "../../Variables/EntityManager.h"
#include "../../Variables/CreatedClass.h"
#include "../../Variables/Entity.h"
#include "../../Variables/Event.h"
#include "../../Variables/ReturnableValue.h"
#include "../../Exceptions/CompilerExceptions.h"


ObjectDefinition::ObjectDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, ClassDefinitionManager* classDefinitionManager, ValueManager* valueManager, EventManager* eventManager, EntityManager* entityManager, FunctionCall* functionCall, ArrayImmediateDefinition* arrayImmediateDefinition)
  : TokenObject(sourceFileIndex) {
  functionCall_ = functionCall;
  arrayImmediateDefinition_ = arrayImmediateDefinition;
  
  
  sourceFile_ = sourceFile;
  classDefinitionManager_ = classDefinitionManager;
  valueManager_ = valueManager;
  eventManager_ = eventManager;
  entityManager_ = entityManager;
}

ObjectDefinition::ObjectDefinition(const ObjectDefinition &original) 
 : TokenObject(original.sourceFileIndex_) {
  functionCall_ = new FunctionCall(*(original.functionCall_));
  arrayImmediateDefinition_ = new ArrayImmediateDefinition(*(original.arrayImmediateDefinition_));
  sourceFile_ = original.sourceFile_;
  classDefinitionManager_ = original.classDefinitionManager_;
  valueManager_ = original.valueManager_;
  eventManager_ = original.eventManager_;
  entityManager_ = original.entityManager_;
}

ObjectDefinition::~ObjectDefinition() {
  delete functionCall_;
  delete arrayImmediateDefinition_;
}

ReturnableValue* ObjectDefinition::returningCode() {
  //this creates a new Object and then returns it in a ReturnableValue object.
  
  if(functionCall_->functionName().compare("Entity") == 0 || functionCall_->functionName().compare("Event") == 0) {
    std::cout << "\033[1;41mERROR:\033[0m Entity and Event are abstract classes. They cannot be instantiated: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::ObjectInstantiationException();
  } else {
    CreatedClass* classToInstantiate = classDefinitionManager_->findClass(functionCall_->functionName());
    if(classToInstantiate == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m " << functionCall_->functionName() << " is not a class: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::ObjectInstantiationException();
    } else {
      //find out whether it is an Event or Entity object
      CreatedClass* baseClass = classToInstantiate->baseClass();
      
      if(baseClass->className().compare("Entity") == 0) {
        //put this object into the EntityManager
        
        Entity* newEntity = new Entity(classToInstantiate, functionCall_->parameterSettingStatement(), arrayImmediateDefinition_);
        
        entityManager_->addEntity(newEntity);
        
        ReturnableValue* returnValue = new ReturnableValue(classToInstantiate->className(), newEntity);
        return returnValue;
      } else if(baseClass->className().compare("Event") == 0) {
        //put this object into the EventManager
        Event* newEvent = new Event(classToInstantiate, functionCall_->parameterSettingStatement(), arrayImmediateDefinition_);
        eventManager_->addEvent(newEvent);
        
        ReturnableValue* returnValue = new ReturnableValue(classToInstantiate->className(), newEvent);
        return returnValue;
      } else {
        std::cout << "COMPILER ERROR: Contact developer. ObjectDefinition.cpp line 50" << std::endl;
      }
    }
  }
  
  return nullptr;
}

ReturnableValue* ObjectDefinition::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  //this creates a new Object and then returns it in a ReturnableValue object.
  std::cout << "\033[1;41mERROR:\033[0m Cannot instantiate an object within a class: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
  throw comp_except::ObjectInstantiationException();
}
