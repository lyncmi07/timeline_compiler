#ifndef MAJOR_OBJECT_GENERATOR_H
#define MAJOR_OBJECT_GENERATOR_H

class VariableManager;
class Token;
class EventManager;
class EntityManager;
class Program;
class TokenObject;
class PreProcessedFile;
class ClassDefinitionManager;
class ValueManager;


class ObjectGenerator {
private:

  //memory not owned
  VariableManager* variableManager_;
  PreProcessedFile* sourceFile_;
  ClassDefinitionManager* classDefinitionManager_;
  ValueManager* valueManager_;
    
  //owned memory
  Program* generatedProgram_;
  EventManager* eventManager_;
  EntityManager* entityManager_;
  


  TokenObject* createTokenObject(Token* token);

public:
  ObjectGenerator(Token* programToken, VariableManager* variableManager, ClassDefinitionManager* classDefinitionManager, ValueManager* valueManager, PreProcessedFile* sourceFile);
  ~ObjectGenerator();
  
  //places the objects created into their respective managers (EntityManager or EventManager)
  //(possible deprecated)
  void manageObjects();
  
  Program* generatedProgram();
  
  EventManager* eventManager();
  EntityManager* entityManager();
  ValueManager* valueManager();
  
};

#endif
