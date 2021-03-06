#ifndef MAJOR_EVENT_H
#define MAJOR_EVENT_H

#include "Object.h"

class MethodDefinition;

class Event : public Object{
private:

public:
  Event(CreatedClass* givenClass, ParameterSettingStatement* constructorParameters, ArrayImmediateDefinition* dependencyArray);
  
  
  int timelineOrderDate() override;
  bool existsAtThisTime(int timeToExist) override;
  
  int eventDate();
  
  MethodDefinition* onEventMethod();
};

#endif
