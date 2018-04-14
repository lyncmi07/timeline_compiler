#ifndef MAJOR_ENTITY_H
#define MAJOR_ENTITY_H

#include "Object.h"

class Entity : public Object {
public:
  Entity(CreatedClass* givenClass, ParameterSettingStatement* construtorParameters, ArrayImmediateDefinition* dependencyArray);
  
  
  int timelineOrderDate() override;
  bool existsAtThisTime(int timeToExist) override;
  
  int startDate();
  int endDate();
};

#endif
