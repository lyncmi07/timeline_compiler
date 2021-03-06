#ifndef MAJOR_PARAMETER_SETTING_STATEMENT
#define MAJOR_PARAMETER_SETTING_STATEMENT

#include "TokenObject.h"
#include <vector>

class Expression;

class ParameterSettingStatement : public TokenObject {
  std::vector<Expression*> allParameters_;
public:
  //PARAMETER_SETTING_STATEMENT
  ParameterSettingStatement(int sourceFileIndex, Expression* expression);
  //PARAMETER_SETTING_STATEMENT2
  ParameterSettingStatement(int sourceFileIndex, Expression* expression, ParameterSettingStatement* parameterSettingStatement);
  
  //copy constructor
  ParameterSettingStatement(const ParameterSettingStatement &original);
  
  ~ParameterSettingStatement();
  
  std::vector<Expression*>* allParameters();
};

#endif
