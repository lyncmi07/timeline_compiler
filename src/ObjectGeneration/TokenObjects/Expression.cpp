#include "Expression.h"
#include "../../Variables/ReturnableValue.h"
#include "SubExpression.h"
#include "ObjectDefinition.h"
#include "Equation.h"

#include <iostream>

Expression::Expression(int sourceFileIndex, SubExpression* subExpression)
  : TokenObject(sourceFileIndex) {
  type_ = E_SE;
  subExpression_ = subExpression;
  objectDefinition_ = nullptr;
  equation_ = nullptr;
}

Expression::Expression(int sourceFileIndex, ObjectDefinition* objectDefinition) 
  : TokenObject(sourceFileIndex) {
  type_ = E_OD;
  subExpression_ = nullptr;
  objectDefinition_ = objectDefinition;
  equation_ = nullptr;
}

Expression::Expression(int sourceFileIndex, Equation* equation)
  : TokenObject(sourceFileIndex) {
  type_ = E_E;
  subExpression_ = nullptr;
  objectDefinition_ = nullptr;
  equation_ = equation;
}

Expression::~Expression() {
  delete subExpression_;
  delete objectDefinition_;
  delete equation_;
}

Expression::Expression(const Expression &original) 
 : TokenObject(original.sourceFileIndex_) {
  type_ = original.type_;
  
  switch(type_) {
  case E_SE:
    subExpression_ = new SubExpression(*(original.subExpression_));
    objectDefinition_ = nullptr;
    equation_ = nullptr;
    break;
  case E_OD:
    subExpression_ = nullptr;
    objectDefinition_ = new ObjectDefinition(*(original.objectDefinition_));
    equation_ = nullptr;
    break;
  case E_E:
    subExpression_ = nullptr;
    objectDefinition_ = nullptr;
    equation_ = new Equation(*(original.equation_));
    break;
  }
}

bool Expression::immediateRun() {
  //this is only false for one particular function.
  if(type_ == E_SE) {
    return subExpression_->immediateRun();
  }
  return true;
}

ReturnableValue* Expression::returningCode() {
  switch(type_) {
  case E_SE:
    return subExpression_->returningCode();
  case E_OD:
    return objectDefinition_->returningCode();
  case E_E:
    return equation_->returningCode();
  }
}

ReturnableValue* Expression::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  switch(type_) {
  case E_SE: {
    ReturnableValue* value = subExpression_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    return value; }
  case E_OD:
    return objectDefinition_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
  case E_E:
    return equation_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
  }
}













