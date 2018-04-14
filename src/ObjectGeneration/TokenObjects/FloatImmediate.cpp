#include "FloatImmediate.h"
#include <math.h>
#include <iostream>


FloatImmediate::FloatImmediate(int sourceFileIndex, int beforeDecimal, std::string afterDecimal)
  : TokenObject(sourceFileIndex) {
  actualValue_ = beforeDecimal;
  int powerValue = afterDecimal.length();
  int divValue = (int)pow(10, powerValue);
  double afterDecimalN = atoi(afterDecimal.c_str());
  afterDecimalN = (afterDecimalN*1.0)/divValue;
  actualValue_ += afterDecimalN;
}

float FloatImmediate::actualValue() {
  return actualValue_;
}
