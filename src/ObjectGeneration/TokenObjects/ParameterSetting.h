#ifndef MAJOR_PARAMETER_SETTING_H
#define MAJOR_PARAMETER_SETTING_H

#include "TokenObject.h"

class Expression;

class ParameterSetting : public TokenObject {
public:
  //LAST_PARAMETER_SETTING | NOT_LAST_PARAMETER_SETTING
  ParameterSetting(int sourceFileIndex, Expression* expression);
};

#endif
