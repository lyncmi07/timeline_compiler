#ifndef MAJOR_EXPRESSION_H
#define MAJOR_EXPRESSION_H

#include "TokenObject.h"
#include "../ValueReturnable.h"
#include "../ClassValueReturnable.h"

class SubExpression;
class ObjectDefinition;
class Equation;
class ReturnableValue;

typedef enum {
  E_SE,
  E_OD,
  E_E
} ExpressionType;

class Expression : public TokenObject, ValueReturnable, ClassValueReturnable {
private: 
  ExpressionType type_;
  //owned memory
  SubExpression* subExpression_;
  ObjectDefinition* objectDefinition_;
  Equation* equation_;
public:
  //EXPRESSION
  Expression(int sourceFileIndex, SubExpression* subExpression);
  //EXPRESSION2
  Expression(int sourceFileIndex, ObjectDefinition* objectDefinition);
  //EXPRESSION3
  Expression(int sourceFileIndex, Equation* equation);
  
  //copy constructor
  Expression(const Expression &original);
  
  ~Expression();
  
  bool immediateRun();
  
  ReturnableValue* returningCode() override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
  
};

#endif
