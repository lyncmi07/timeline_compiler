#include "Event.h"

#include <iostream>
#include "ReturnableValue.h"
#include "DynamicReturnableValue.h"
#include "VariableManager.h"
#include "Variable.h"
#include "CreatedClass.h"
#include "MethodManager.h"

Event::Event(CreatedClass* givenClass, ParameterSettingStatement* constructorParameters, ArrayImmediateDefinition* dependencyArray)
  : Object(givenClass, constructorParameters, dependencyArray) {

}

int Event::timelineOrderDate() {
  ReturnableValue* value = variableManager_->getVariable("eventYear")->value();
  if(value == nullptr || value->valueType().compare("int") != 0) {
    std::cout << "COMPILER ERROR: contact developer: Event.cpp line 13" << std::endl;
    return -1;
  } else {
    return value->integerValue();
  }
}

bool Event::existsAtThisTime(int timeToExist) {
  if(eventDate() <= timeToExist) {
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
        } else if(!testObject->existsAtThisTime(eventDate())) {
          return false;
        }
      } else {
        if(!dependencies()->at(i)->objectValue()->existsAtThisTime(eventDate())) {
          return false;
        }
      }
    }
    return true;
  } else {
    return false;
  }
}

int Event::eventDate() {
  return variableManager()->getVariable("eventYear")->value()->integerValue();
}

MethodDefinition* Event::onEventMethod() {
  return givenClass()->methods()->getMethod("onEvent");
}
