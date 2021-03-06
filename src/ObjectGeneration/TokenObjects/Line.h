#ifndef MAJOR_LINE_H
#define MAJOR_LINE_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include "../ClassRunnable.h"
#include "../ClassValueReturnable.h"

class Statement;
class ReturnableValue;

class Line : public TokenObject, CodeRunnable, ClassRunnable, ClassValueReturnable {
private:
  //owned memory
  Statement* statement_;
public:
  //LINE
  Line(int sourceFileIndex, Statement* statement);
  
  //copy constructor
  Line(const Line &original);
  
  ~Line();
  
  void runCode() override;
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
