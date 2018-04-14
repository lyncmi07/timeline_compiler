#include "StringFunctions.h"

namespace str_func {
  std::string removeWhitespace(std::string originalString) {
    std::string newValue;
    for(unsigned int i = 0; i < originalString.length(); i++) {
      //don't add if (>=9 <=13) (=32) these are whitespace characters
      if((originalString[i] >= 9 && originalString[i] <= 13) || originalString[i] == 32) {
        //do not add
      } else {
        newValue.push_back(originalString[i]);
      }
    }
    return newValue;
  }
}
