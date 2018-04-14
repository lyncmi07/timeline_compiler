#ifndef MAJOR_VARIABLE_MANAGER_H
#define MAJOR_VARIABLE_MANAGER_H

#include <vector>
#include <string>


class Variable;
class Expression;

class VariableManager {
private:
  //owned memory
  std::vector<Variable*> allVariables_;
  
  //memory not owned
  //When running methods, any VariableInitiations are initiatiated into this manager to be deleted when the method finishes running
  VariableManager* localVariableManager_;
  
  std::vector<VariableManager*> localManagers_;
  std::vector<int> localManagerStack_;
  
  bool checkVariableExistence(std::string identifier);
  
public:

  VariableManager();
  
  //copy constructor
  VariableManager(const VariableManager &original);
  
  ~VariableManager();

  std::vector<Variable*>* allVariables();
  
  
  
  
  
  //get an individual variable by its identifier
  Variable* getVariable(std::string identifier);
  
  //returns the variable is added if successfully added. nullptr otherwise
  Variable* addVariable(std::string variableType, std::string identifier);
  Variable* addVariable(std::string variableType, std::string identifier, Expression* value);
  
  //all new VariableInitiations are written into the local variable manager
  void addLocalVariableManager(VariableManager* localVariableManager);
  void removeLocalVariableManager();
  
  VariableManager* localVariableManager();
  
  //for debug purposes write out all variables with their corresponding value
  void listGeneratedVariables();
  
  std::string printToXML(int spaceIndent);

};

#endif
