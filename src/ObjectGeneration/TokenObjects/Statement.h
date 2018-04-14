#ifndef MAJOR_STATEMENT_H
#define MAJOR_STATEMENT_H

#include "TokenObject.h"
#include "../CodeRunnable.h"
#include "../ClassRunnable.h"
#include "../ClassValueReturnable.h"

class VariableDefinition;
class VariableInitiation;
class FunctionCall;
class SubVariableIdentifier;
class ReturnableValue;

typedef enum {
  ST_VD,
  ST_VI,
  ST_FC,
  ST_SVI
} StatementType;

class Statement : public TokenObject, CodeRunnable, ClassRunnable, ClassValueReturnable {
private:
  StatementType type_;
  
  //owned memory
  VariableDefinition* variableDefinition_;
  VariableInitiation* variableInitiation_;
  FunctionCall* functionCall_;
  SubVariableIdentifier* subVariableIdentifier_;
public:
  //STATEMENT
  Statement(int sourceFileIndex, VariableDefinition* variableDefinition);
  //STATEMENT2
  Statement(int sourceFileIndex, VariableInitiation* variableInitiation);
  //STATEMENT3
  Statement(int sourceFileIndex, FunctionCall* functionCall);
  //STATEMENT4
  Statement(int sourceFileIndex, SubVariableIdentifier* subVariableIdentifier);
  
  //copy constructor
  Statement(const Statement &original);
  
  ~Statement();
  
  void runCode() override;
  void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
