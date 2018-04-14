#ifndef MAJOR_PARAMETER_DEFINITION_STATEMENT_H
#define MAJOR_PARAMETER_DEFINITION_STATEMENT_H

#include "TokenObject.h"

#include <vector>

class VariableInitiation;

class ParameterDefinitionStatement : public TokenObject {
private:
  //owned memory
  std::vector<VariableInitiation*> allParameters_;

public:
  //PARAMETER_DEFINITION_STATEMENT
  ParameterDefinitionStatement(int sourceFileIndex);
  //PARAMETER_DEFINITION_STATEMENT_FILLED
  ParameterDefinitionStatement(int sourceFileIndex, VariableInitiation* parameterDefinition);
  //PARAMETER_DEFINITION_STATEMENT_FILLED2
  ParameterDefinitionStatement(int sourceFileIndex, VariableInitiation* parameterDefinition, ParameterDefinitionStatement* parameterDefinitionStatement);
  
  //copy constructor
  ParameterDefinitionStatement(const ParameterDefinitionStatement &original);
  
  ~ParameterDefinitionStatement();
  
  std::vector<VariableInitiation*>* allParameters();
};

#endif
