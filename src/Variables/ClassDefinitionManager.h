#ifndef MAJOR_CLASS_DEFINITION_MANAGER
#define MAJOR_CLASS_DEFINITION_MANAGER

#include <vector>
#include <string>

class CreatedClass;
class ClassDefinition;

class ClassDefinitionManager {
private:
  //owned memory
  std::vector<CreatedClass*> allClasses_;
public:
  ClassDefinitionManager();
  ~ClassDefinitionManager();
  
  void addNewClassDefinition(ClassDefinition* newClassDefinition);
  CreatedClass* findClass(std::string className);
  
  void listClasses();
  
};

#endif
