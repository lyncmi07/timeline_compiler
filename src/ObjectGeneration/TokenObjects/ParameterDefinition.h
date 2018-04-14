#ifndef MAJOR_PARAMETER_DEFINITION_H
#define MAJOR_PARAMETER_DEFINITION_H

#include "TokenObject.h"

class VariableInitiation;

class ParameterDefinition : public TokenObject {
public:
  //LAST_PARAMETER_DEFINITION | NOT_LAST_PARAMETER_DEFINITION
  ParameterDefinition(int sourceFileIndex, VariableInitiation* variableInitiation);
};

#endif
