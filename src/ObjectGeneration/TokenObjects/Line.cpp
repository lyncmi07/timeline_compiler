#include "Line.h"
#include "Statement.h"

Line::Line(int sourceFileIndex, Statement* statement)
  : TokenObject(sourceFileIndex) {
  statement_ = statement;
}

Line::Line(const Line &original) 
 : TokenObject(original.sourceFileIndex_) {
  statement_ = new Statement(*(original.statement_));
}

Line::~Line() {
  delete statement_;
}

void Line::runCode() {
  statement_->runCode();
}

void Line::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  statement_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
}

ReturnableValue* Line::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  return statement_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
}
