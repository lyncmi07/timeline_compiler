#ifndef MAJOR_CREATED_CLASS_H
#define MAJOR_CREATED_CLASS_H

#include <string>

class ConstructionDefinition;
class VariableManager;
class MethodManager;

class CreatedClass {
private:
  //owned memory
  std::string className_;
  ConstructionDefinition* constructionDefinition_;
  VariableManager* variableManager_;
  MethodManager* methodManager_;
  
  //memory not owned
  CreatedClass* parentClass_;
public:
  CreatedClass(std::string className, VariableManager* subVariables, MethodManager* methods, ConstructionDefinition* constructionDefinition, CreatedClass* parentClass);
  ~CreatedClass();

  ConstructionDefinition* constructionDefinition();
  std::string className();
  VariableManager* subVariables();
  MethodManager* methods();
  CreatedClass* parentClass();
  
  //finds the base class (Either Entity of Event) or this class
  CreatedClass* baseClass();
};

#endif
