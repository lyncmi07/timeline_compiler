#ifndef MAJOR_DYNAMIC_RETURNABLE_VALUE_H
#define MAJOR_DYNAMIC_RETURNABLE_VALUE_H

#include "ReturnableValue.h"

#include <string>

class CreatedClass;
class PossibleObject;
class SubVariableIdentifier;
class EventManager;
class Event;

typedef enum {
  DRV_O,
  DRV_PO,
  DRV_SVI
} DynamicType;

class DynamicReturnableValue : public ReturnableValue {
private:
  //memory not owned
  CreatedClass* givenClass_;
  std::string testingSubVariable_;
  ReturnableValue* comparisonValue_;
  EventManager* eventManager_;
  
  DynamicType type_;
  std::string realValueType_;
  
  bool readyForUse_;
  
  //memory owned
  ReturnableValue* ownValue_;
  
  //memory not owned
  DynamicReturnableValue* parentValue_;
  PossibleObject* subVariable_;
  SubVariableIdentifier* subTree_;
  ReturnableValue* actualValue_;
  
  Event* getEvent();
public:
  DynamicReturnableValue(EventManager* eventManager, CreatedClass* givenClass, std::string testingSubVariable, ReturnableValue* comparisonValue);
  DynamicReturnableValue(DynamicReturnableValue* parentValue, PossibleObject* subVariable);
  DynamicReturnableValue(DynamicReturnableValue* parentValue, SubVariableIdentifier* subTree);
  
  ~DynamicReturnableValue();
  
  std::string realValueType();
  
  std::string stringLiteralValue() override;
  int integerValue() override;
  float floatValue() override;
  Object* objectValue() override;
  
  EventManager* eventManager();
  
  void makeReady();
  bool readyForUse();
  void updateValue();
  
  bool isObject() override;
  
  std::string toString() override;
  
  std::string printToXML(int spaceIndent) override;
};

#endif
