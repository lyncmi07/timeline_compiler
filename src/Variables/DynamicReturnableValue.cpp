#include "DynamicReturnableValue.h"

#include <iostream>

#include "CreatedClass.h"
#include "VariableManager.h"
#include "Variable.h"
#include "EventManager.h"
#include "Event.h"
#include "../ObjectGeneration/TokenObjects/PossibleObject.h"
#include "../ObjectGeneration/TokenObjects/SubVariableIdentifier.h"
#include "../Exceptions/CompilerExceptions.h"

DynamicReturnableValue::DynamicReturnableValue(EventManager* eventManager, CreatedClass* givenClass, std::string testingSubVariable, ReturnableValue* comparisonValue) 
 : ReturnableValue("dynamic", nullptr) {
  type_ = DRV_O;
  givenClass_ = givenClass;
  testingSubVariable_ = testingSubVariable;
  comparisonValue_ = comparisonValue;
  eventManager_ = eventManager;
  
  realValueType_ = givenClass->className();
  readyForUse_ = false;
  ownValue_ = nullptr;
}

DynamicReturnableValue::DynamicReturnableValue(DynamicReturnableValue* parentValue, PossibleObject* subVariable)
 : ReturnableValue("dynamic_svi", nullptr) {
  type_ = DRV_PO;
  parentValue_ = parentValue;
  subVariable_ = subVariable;
  eventManager_ = parentValue->eventManager_;
  givenClass_ = parentValue->givenClass_;
  testingSubVariable_ = parentValue->testingSubVariable_;
  comparisonValue_ = parentValue->comparisonValue_;
  realValueType_ = subVariable->getVariable(givenClass_->subVariables())->variableType();
  readyForUse_ = false;
  ownValue_ = nullptr;
}

DynamicReturnableValue::DynamicReturnableValue(DynamicReturnableValue* parentValue, SubVariableIdentifier* subTree) 
 : ReturnableValue("dynamic_svi", nullptr) {
  type_ = DRV_SVI;
  parentValue_ = parentValue;
  subTree_ = subTree;
  eventManager_ = parentValue->eventManager_;
  givenClass_ = parentValue->givenClass_;
  testingSubVariable_ = parentValue->testingSubVariable_;
  comparisonValue_ = parentValue->comparisonValue_;
  realValueType_ = "UNKNOWN";
  readyForUse_ = false;
  ownValue_ = nullptr;
}

DynamicReturnableValue::~DynamicReturnableValue() {
  delete ownValue_;
}

std::string DynamicReturnableValue::realValueType() {
  return realValueType_;
}

std::string DynamicReturnableValue::stringLiteralValue() {
  switch(type_) {
    case DRV_O:
      std::cout << "\033[1;41mERROR:\033[0m incorrect datatype. Should be string not " << realValueType_ << std::endl;
      break;
    case DRV_PO:
      return actualValue_->stringLiteralValue();
    case DRV_SVI:
      return actualValue_->stringLiteralValue();
  }
}

int DynamicReturnableValue::integerValue() {
  switch(type_) {
    case DRV_O:
      std::cout << "\033[1;41mERROR:\033[0m incorrect datatype. Should be string not " << realValueType_ << std::endl;
      break;
    case DRV_PO:
      return actualValue_->integerValue();
    case DRV_SVI:
      return actualValue_->integerValue();
  }
}

float DynamicReturnableValue::floatValue() {
  switch(type_) {
    case DRV_O:
      std::cout << "\033[1;41mERROR:\033[0m incorrect datatype. Should be string not " << realValueType_ << std::endl;
      break;
    case DRV_PO:
      return actualValue_->floatValue();
    case DRV_SVI:
      return actualValue_->floatValue();
  }
}

Object* DynamicReturnableValue::objectValue() {
  switch(type_) {
    case DRV_O:
      if(ownValue_ == nullptr) {
        return nullptr;
      }
      return ownValue_->objectValue();
    case DRV_PO:
      if(actualValue_ == nullptr) {
        return nullptr;
      }
      return actualValue_->objectValue();
    case DRV_SVI:
      if(actualValue_ == nullptr) {
        return nullptr;
      }
      actualValue_->objectValue();
  }
}

void DynamicReturnableValue::updateValue() {
  if(readyForUse_) {
    switch(type_) {
      case DRV_O: {
        Event* currentEvent = getEvent();
        if(currentEvent == nullptr) {
          realValueType_ = "UNKNOWN";
          delete ownValue_;
          ownValue_ = nullptr;
          return;
        }
        delete ownValue_;
        ownValue_ = new ReturnableValue(givenClass_->className(), currentEvent);
        realValueType_ = ownValue_->valueType();
        break; }
      case DRV_PO: {
        Event* currentEvent = getEvent();
        std::cout << "past event" << std::endl;
        if(currentEvent == nullptr) {
          realValueType_ = "UNKNOWN";
          actualValue_ = nullptr;
          return;
        }
        ReturnableValue* value =  subVariable_->returningCodeOutsideClassMode(currentEvent->variableManager(),
                                                                            givenClass_->methods(),
                                                                            givenClass_->parentClass());
        realValueType_ = value->valueType();
        actualValue_ = value;                                        
        break; }
      case DRV_SVI: {
        Event* currentEvent = getEvent();
        if(currentEvent == nullptr) {
          realValueType_ = "UNKNOWN";
          actualValue_ = nullptr;
          return;
        }
        ReturnableValue* value =  subTree_->returningCodeOutsideClassMode(currentEvent->variableManager(),
                                                                            givenClass_->methods(),
                                                                            givenClass_->parentClass());
        realValueType_ = value->valueType();
        actualValue_ = value;    
        break; }
    }
  }
}

std::string DynamicReturnableValue::toString() {
  return "dynamic-value";
}

void DynamicReturnableValue::makeReady() {
  readyForUse_ = true;
}

bool DynamicReturnableValue::readyForUse() {
  return readyForUse_;
}

bool DynamicReturnableValue::isObject() {
  if(valueType().compare("dynamic") == 0) {
    //this is always an object
    return true;
  } else {
    //TODO: this can be worked out. try and do the checks here, for now keep it as true
    return true;
  }
}

Event* DynamicReturnableValue::getEvent() {
  for(int i = 0; i < eventManager_->allEvents()->size(); i++) {
    if(eventManager_->allEvents()->at(i)->givenClass() == givenClass_) {
      Variable* var = eventManager_->allEvents()->at(i)->variableManager()->getVariable(testingSubVariable_);
      if(var == nullptr) {
        std::cout << "COMPILER ERROR: contact developer. DynamicReturnableValue.cpp line 133" << std::endl;
      } else {
        //in correct place
        if(((*comparisonValue_) == (*var->value())) && eventManager_->allEvents()->at(i)->happens()) {
          return eventManager_->allEvents()->at(i);
        }
      }
    }
  }
  std::cout << "\033[1;41mERROR:\033[0m no events available using these settings for the GetEventByDataValue function" << std::endl;
  //throw comp_except::MethodCallException();
  return nullptr;
}

std::string DynamicReturnableValue::printToXML(int spaceIndent) {
  std::cout << "reached this point" << std::endl;
  updateValue();
  if(type_ == DRV_O) {
    if(ownValue_ == nullptr) {
      return "NULL";
    }
    return ownValue_->printToXML(spaceIndent);
  } else {
    if(actualValue_ == nullptr) {
      return "NULL";
    }
    return actualValue_->printToXML(spaceIndent);
  }
}

