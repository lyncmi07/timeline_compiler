#include "ParameterSettingStatement.h"
#include "Expression.h"

ParameterSettingStatement::ParameterSettingStatement(int sourceFileIndex, Expression* expression)
  : TokenObject(sourceFileIndex) {
  allParameters_.push_back(expression);
}

ParameterSettingStatement::ParameterSettingStatement(int sourceFileIndex, Expression* expression, ParameterSettingStatement* parameterSettingStatement)
  : TokenObject(sourceFileIndex) {
  allParameters_.push_back(expression);
  for(int i = 0; i < parameterSettingStatement->allParameters()->size(); i++) {
    allParameters_.push_back(parameterSettingStatement->allParameters()->at(i));
  }
  
  parameterSettingStatement->allParameters()->clear();
  
  delete parameterSettingStatement;
}

ParameterSettingStatement::ParameterSettingStatement(const ParameterSettingStatement &original)
 : TokenObject(original.sourceFileIndex_) {
  for(int i = 0; i < original.allParameters_.size(); i++) {
    allParameters_.push_back(new Expression(*(original.allParameters_[i])));
  }
}

ParameterSettingStatement::~ParameterSettingStatement() {
  for(int i = 0; i < allParameters_.size(); i++) {
    delete allParameters_[i];
  }
}

std::vector<Expression*>* ParameterSettingStatement::allParameters() {
  return &allParameters_;
}
