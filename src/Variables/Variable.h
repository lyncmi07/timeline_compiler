#ifndef MAJOR_VARIABLE_H
#define MAJOR_VARIABLE_H

#include <string>

class Expression;
class ReturnableValue;
class VariableManager;
class MethodManager;
class CreatedClass;

class Variable {
private:
  std::string type_;
  std::string identifier_;
  
  //owned memory
  ReturnableValue* value_;
  
  //memory not owned
  Expression* valueGenerator_;
  bool valueSet_;
  bool immediateRun_;
  bool inClass_;
  VariableManager* classVariableManager_;
  MethodManager* classMethodManager_;
public:
  Variable(std::string type, std::string identifier);
  
  ~Variable();
  
  void setValue(Expression* valueGenerator);
  void setValue(ReturnableValue* actualValue);
  void setValueInsideClass(Expression* valueGenerator, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass);
  ReturnableValue* value();
  
  std::string variableType();
  
  std::string identifier();
  
  std::string toString();
  
  std::string printToXML(int spaceIndent);

};

#endif
