#ifndef MAJOR_ARRAY_IMMEDIATE_DEFINITION_H
#define MAJOR_ARRAY_IMMEDAITE_DEFINITION_H

#include "TokenObject.h"
#include <vector>


class ParameterSettingStatement;
class Expression;

class ArrayImmediateDefinition : public TokenObject {
private:
  //owned memory
  ParameterSettingStatement* parameterSettingStatement_;

public:
  //ARRAY_IMMEDIATE_DEFINITION
  ArrayImmediateDefinition(int sourceFileIndex, ParameterSettingStatement* parameterSettingStatement);
  //ARRAY_IMMEDIATE_DEFINITION2
  ArrayImmediateDefinition(int sourceFileIndex);
  
  //copy constructor
  ArrayImmediateDefinition(const ArrayImmediateDefinition &original);
  
  ~ArrayImmediateDefinition();
  
  std::vector<Expression*>* allParameters();
};

#endif
