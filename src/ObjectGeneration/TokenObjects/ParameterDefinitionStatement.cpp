#include "ParameterDefinitionStatement.h"

#include "VariableInitiation.h"
#include <iostream>
ParameterDefinitionStatement::ParameterDefinitionStatement(int sourceFileIndex) 
  : TokenObject(sourceFileIndex) {
  
}

ParameterDefinitionStatement::ParameterDefinitionStatement(int sourceFileIndex, VariableInitiation* parameterDefinition)
  : TokenObject(sourceFileIndex) {
  allParameters_.push_back(parameterDefinition);
}

ParameterDefinitionStatement::ParameterDefinitionStatement(int sourceFileIndex, VariableInitiation* parameterDefinition, ParameterDefinitionStatement* parameterDefinitionStatement)
  : TokenObject(sourceFileIndex) {
  allParameters_.push_back(parameterDefinition);
  
  for(int i = 0; i < parameterDefinitionStatement->allParameters()->size(); i++) {
    allParameters_.push_back(parameterDefinitionStatement->allParameters()->at(i));
  }
  
  parameterDefinitionStatement->allParameters()->clear();
  
  delete parameterDefinitionStatement;
}

ParameterDefinitionStatement::ParameterDefinitionStatement(const ParameterDefinitionStatement &original) 
 : TokenObject(original.sourceFileIndex_) {
  for(int i = 0; i < original.allParameters_.size(); i++) {
    allParameters_.push_back(new VariableInitiation(*(original.allParameters_[i])));
  }
}

ParameterDefinitionStatement::~ParameterDefinitionStatement() {
  for(int i = 0; i < allParameters_.size(); i++) {
    delete allParameters_[i];
  }
}

std::vector<VariableInitiation*>* ParameterDefinitionStatement::allParameters() {
  return &allParameters_;
}
