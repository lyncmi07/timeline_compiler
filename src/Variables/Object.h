#ifndef MAJOR_OBJECT_H
#define MAJOR_OBJECT_H

#include <vector>
#include <string>

class CreatedClass;
class VariableManager;
class ArrayImmediateDefinition;
class ParameterSettingStatement;
class ReturnableValue;

class Object {
protected:
  //owned memory
  VariableManager* variableManager_;
  
  //memory not owned
  CreatedClass* givenClass_;
  std::vector<ReturnableValue*> dependencies_;
  
  bool happens_;
  bool processed_;
public:

  Object(CreatedClass* givenClass, ParameterSettingStatement* constructorParameters, ArrayImmediateDefinition* dependencyArray);

  virtual ~Object();
  virtual int timelineOrderDate() = 0;
  virtual bool existsAtThisTime(int timeToExist) = 0;
  
  std::string toString();
  
  VariableManager* variableManager();
  CreatedClass* givenClass();
  
  std::vector<ReturnableValue*>* dependencies();
  
  bool happens();
  void setHappens(bool doesItHappen);
  
  bool processed();
  void finishedProcessing();
  
  std::string printToXML(int spaceIndent);
};

#endif
