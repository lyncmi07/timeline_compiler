#include "Entity.h"

#include <iostream>
#include "ReturnableValue.h"
#include "DynamicReturnableValue.h"
#include "VariableManager.h"
#include "Variable.h"

Entity::Entity(CreatedClass* givenClass, ParameterSettingStatement* constructorParameters, ArrayImmediateDefinition* dependencyArray) 
 : Object(givenClass, constructorParameters, dependencyArray) {
 
}

int Entity::timelineOrderDate() {
  ReturnableValue* value = variableManager_->getVariable("startYear")->value();
  if(value == nullptr || value->valueType().compare("int") != 0) {
    std::cout << "COMPILER ERROR: contact developer: Entity.cpp line 61" << std::endl;
  } else {
    return value->integerValue();
  }
}

bool Entity::existsAtThisTime(int timeToExist) {
  if(startDate() <= timeToExist &&
     endDate() >= timeToExist) {
    for(int i = 0; i < dependencies()->size(); i++) {
      if(dependencies()->at(i)->valueType().compare("dynamic_svi") == 0 || dependencies()->at(i)->valueType().compare("dynamic") == 0) {
        //this won't have the correct value type until it has been run
        ((DynamicReturnableValue*)dependencies()->at(i))->updateValue();
        Object* testObject = dependencies()->at(i)->objectValue();
        if(testObject == nullptr) {
          return false;
        }
        if(((DynamicReturnableValue*)dependencies()->at(i))->realValueType().compare("string") == 0 ||
           ((DynamicReturnableValue*)dependencies()->at(i))->realValueType().compare("int") == 0 ||
           ((DynamicReturnableValue*)dependencies()->at(i))->realValueType().compare("float") == 0) {
          std::cout << "ERROR: Can only be dependent on other objects" << std::endl;
          return false;
        } else if(!testObject->existsAtThisTime(startDate())) {
          return false;
        }
      } else {
        if(!dependencies()->at(i)->objectValue()->existsAtThisTime(startDate())) {
          return false;
        }
      }
    }
    return true;
  } else {
    return false;
  }
}

int Entity::startDate() {
  return variableManager()->getVariable("startYear")->value()->integerValue();
}

int Entity::endDate() {
  return variableManager()->getVariable("endYear")->value()->integerValue();
}
