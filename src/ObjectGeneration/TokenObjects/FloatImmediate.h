#ifndef MAJOR_FLOAT_IMMEDIATE_H
#define MAJOR_FLOAT_IMMEDIATE_H

#include "TokenObject.h"
#include <string>

class FloatImmediate : public TokenObject{
private:
  float actualValue_;

public:
  //FLOAT_IMMEDIATE
  FloatImmediate(int sourceFileIndex, int beforeDecimal, std::string afterDecimal);
  
  float actualValue();
};

#endif
