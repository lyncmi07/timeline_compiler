#include "ClassDefinitionManager.h"

#include "../ObjectGeneration/TokenObjects/ClassDefinition.h"
#include "../ObjectGeneration/TokenObjects/MethodDefinition.h"
#include "../ObjectGeneration/TokenObjects/ClassCodeBlock.h"
#include "../ObjectGeneration/TokenObjects/ParameterDefinitionStatement.h"
#include "VariableManager.h";
#include "CreatedClass.h"
#include "MethodManager.h"
#include "Variable.h"
#include <iostream>
#include "../Helpers/StringFunctions.h"
#include "../Exceptions/CompilerExceptions.h"
#include <iostream>

ClassDefinitionManager::ClassDefinitionManager() {
  std::string entityClassName = "Entity";
  VariableManager* entityVariableManager = new VariableManager();
  entityVariableManager->addVariable("string", "name");
  entityVariableManager->addVariable("int", "startYear");
  entityVariableManager->addVariable("int", "endYear");
  MethodManager* entityMethodManager = new MethodManager();
  allClasses_.push_back(new CreatedClass(entityClassName, 
                                         entityVariableManager, 
                                         entityMethodManager, 
                                         nullptr,
                                         nullptr));
                                         
  std::string eventClassName = "Event";
  VariableManager* eventVariableManager = new VariableManager();
  eventVariableManager->addVariable("string", "name");
  eventVariableManager->addVariable("int", "eventYear");
  MethodManager* eventMethodManager = new MethodManager();
  eventMethodManager->addMethod(new MethodDefinition(0, nullptr, "empty", "onEvent", new ParameterDefinitionStatement(0), nullptr));
  allClasses_.push_back(new CreatedClass(eventClassName,
                                         eventVariableManager,
                                         eventMethodManager,
                                         nullptr,
                                         nullptr));
}

ClassDefinitionManager::~ClassDefinitionManager() {
  for(int i = 0; i < allClasses_.size(); i++) {
    delete allClasses_[i];
  }
}

void ClassDefinitionManager::addNewClassDefinition(ClassDefinition* newClassDefinition) {
  std::string className = newClassDefinition->className();
  className = str_func::removeWhitespace(className);
  std::string parentClassName = str_func::removeWhitespace(newClassDefinition->parentClassName());
  CreatedClass* parentClass = findClass(parentClassName);
  VariableManager* variableManager = new VariableManager();
  if(parentClass != nullptr) {
    for(int i = 0; i < parentClass->subVariables()->allVariables()->size(); i++) {
      variableManager->addVariable(parentClass->subVariables()->allVariables()->at(i)->variableType(), parentClass->subVariables()->allVariables()->at(i)->identifier());
    }
  } else {
    std::cout << "\033[1;41mERROR:\033[0m Parent Class '" << newClassDefinition->parentClassName() << "' of '" << className << "' does not exist" << std::endl;
    throw comp_except::StructureException();
  }
  
  MethodManager* methodManager = new MethodManager(*(parentClass->methods()));

  ConstructionDefinition* constructionDefinition = nullptr;

  if(newClassDefinition->classCodeBlock() != nullptr) {  
    newClassDefinition->classCodeBlock()->runCodeClassMode(variableManager, methodManager, parentClass);
    constructionDefinition = newClassDefinition->classCodeBlock()->constructionDefinition();
  }
  
  allClasses_.push_back(new CreatedClass(className, variableManager, methodManager, constructionDefinition, parentClass));
}

CreatedClass* ClassDefinitionManager::findClass(std::string className) {
  for(int i = 0; i < allClasses_.size(); i++) {
    if(allClasses_[i]->className().compare(className) == 0) {
      //this is the class they were looking for
      return allClasses_[i];
    }
  }
  return nullptr;
  
}

void ClassDefinitionManager::listClasses() {
  for(int i = 0; i < allClasses_.size(); i++) {
    if(allClasses_[i]->parentClass() == nullptr) {
      std::cout << "Base Class: " << allClasses_[i]->className() << std::endl;
    } else {
      std::cout << "Class:      " << allClasses_[i]->className() << " extends " << allClasses_[i]->parentClass()->className() << std::endl;
      std::cout << "__" << allClasses_[i]->className() << " Variables__" << std::endl;
      allClasses_[i]->subVariables()->listGeneratedVariables();
      std::cout << std::endl << "__" << allClasses_[i]->className() << " Methods__" << std::endl;
      allClasses_[i]->methods()->listMethods();
      std::cout << std::endl;
    }
  }
}
