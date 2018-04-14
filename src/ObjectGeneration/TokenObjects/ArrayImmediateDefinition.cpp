#include "ArrayImmediateDefinition.h"

#include "ParameterSettingStatement.h"

#include <iostream>

ArrayImmediateDefinition::ArrayImmediateDefinition(int sourceFileIndex, ParameterSettingStatement* parameterSettingStatement) 
  : TokenObject(sourceFileIndex) {
  parameterSettingStatement_ = parameterSettingStatement;
}

ArrayImmediateDefinition::ArrayImmediateDefinition(int sourceFileIndex)
  : TokenObject(sourceFileIndex) {
  parameterSettingStatement_ = nullptr;
}

ArrayImmediateDefinition::ArrayImmediateDefinition(const ArrayImmediateDefinition &original) 
 : TokenObject(original.sourceFileIndex_) {
  parameterSettingStatement_ = new ParameterSettingStatement(*(original.parameterSettingStatement_));
}

ArrayImmediateDefinition::~ArrayImmediateDefinition() {
  delete parameterSettingStatement_;
}

std::vector<Expression*>* ArrayImmediateDefinition::allParameters() {
  if(parameterSettingStatement_ != nullptr) {
    return parameterSettingStatement_->allParameters();
  } else {
    return nullptr;
  }
}
