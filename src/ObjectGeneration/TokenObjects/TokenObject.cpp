#include "TokenObject.h"

TokenObject::TokenObject(int sourceFileIndex) {
  sourceFileIndex_ = sourceFileIndex;
}

int TokenObject::sourceFileIndex() {
  return sourceFileIndex_;
}
