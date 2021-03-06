#include "ValueManager.h"
#include "ReturnableValue.h"
#include "DynamicReturnableValue.h"

ValueManager::ValueManager() {
  dynamicValuesReady_ = false;
}

ValueManager::~ValueManager() {
  for(int i = 0; i < allValues_.size(); i++) {
    delete allValues_[i];
  }
}

ReturnableValue* ValueManager::newValue(std::string stringLiteralValue) {
  ReturnableValue* value = new ReturnableValue(stringLiteralValue);
  allValues_.push_back(value);
  
  return value;
}

ReturnableValue* ValueManager::newValue(int integerValue) {
  ReturnableValue* value = new ReturnableValue(integerValue);
  allValues_.push_back(value);
  
  return value;
}

ReturnableValue* ValueManager::newValue(float floatValue) {
  ReturnableValue* value = new ReturnableValue(floatValue);
  allValues_.push_back(value);
  
  return value;
}

ReturnableValue* ValueManager::newValue(std::string valueType, Object* objectValue) {
  ReturnableValue* value = new ReturnableValue(valueType, objectValue);
  allValues_.push_back(value);
  
  return value;
}

void ValueManager::newValue(ReturnableValue* customValue) {
  allValues_.push_back(customValue);
}

void ValueManager::allowDynamicValueUse() {
  dynamicValuesReady_ = true;
  for(int i = 0; i < allValues_.size(); i++) {
    if(allValues_[i]->valueType().compare("dynamic") == 0 || allValues_[i]->valueType().compare("dynamic_svi") == 0) {
      ((DynamicReturnableValue*)allValues_[i])->makeReady();
    }
  }
}

bool ValueManager::dynamicValuesCanBeUsed() {
  return dynamicValuesReady_;
}
