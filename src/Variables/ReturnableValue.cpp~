#include "ReturnableValue.h"
#include "Object.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

ReturnableValue::ReturnableValue(std::string stringLiteralValue) {
  valueType_ = "string";
  stringLiteralValue_ = stringLiteralValue;
  integerValue_ = -1;
  floatValue_ = -1;
  objectValue_ = nullptr;
  isObject_ = false;
}

ReturnableValue::ReturnableValue(int integerValue) {
  valueType_ = "int";
  integerValue_ = integerValue;
  floatValue_ = -1;
  objectValue_ = nullptr;
  isObject_ = false;
}

ReturnableValue::ReturnableValue(float floatValue) {
  valueType_ = "float";
  integerValue_ = -1;
  floatValue_ = floatValue;
  objectValue_ = nullptr;
  isObject_ = false;
}

ReturnableValue::ReturnableValue(std::string valueType, Object* objectValue) {
  valueType_ = valueType;
  objectValue_ = objectValue;
  isObject_ = true;
  integerValue_ = -1;
  floatValue_ = -1;
}

ReturnableValue::~ReturnableValue() {

}

std::string ReturnableValue::stringLiteralValue() {
  return stringLiteralValue_;
}

int ReturnableValue::integerValue() {
  return integerValue_;
}

float ReturnableValue::floatValue() {
  return floatValue_;
}

Object* ReturnableValue::objectValue() {
  return objectValue_;
}

std::string ReturnableValue::valueType() {
  return valueType_;
}

std::string ReturnableValue::toString() {
  if(valueType_.compare("string") == 0) {
    return "\"" + stringLiteralValue_ + "\"";
  } else if(valueType_.compare("int") == 0) {
    return std::to_string(integerValue_);
  } else if(valueType_.compare("float") == 0) {
    return std::to_string(floatValue_);
  } else {
    if(objectValue_ == nullptr) {
      return "nullptr";
    } else {
      return objectValue_->toString();
    }
  }
}

bool ReturnableValue::isObject() {
  return isObject_;
}

bool ReturnableValue::operator==(const ReturnableValue &comparison) const {
  //the two values must have the same value type to be the same
  if(valueType_.compare(comparison.valueType_) == 0) {
    if(valueType_.compare("string") == 0) {
      if(stringLiteralValue_.compare(comparison.stringLiteralValue_) == 0) {
        return true;
      }
    } else if(valueType_.compare("int") == 0) {
      if(integerValue_ == comparison.integerValue_) {
        return true;
      }
    } else if(valueType_.compare("float") == 0) {
      if(floatValue_ == comparison.floatValue_) {
        return true;
      }
    } else {
      //if they point to the same place then it is the same object
      if(objectValue_ == comparison.objectValue_) {
        return true;
      }
    }
  }
  return false;
}

std::string ReturnableValue::printToXML(int spaceIndent) {
  std::string returnString = "";
  if(valueType_.compare("string") == 0) {
    returnString += stringLiteralValue_;
  } else if(valueType_.compare("int") == 0) {
    returnString += std::to_string(integerValue_);
  } else if(valueType_.compare("float") == 0) {
    returnString += std::to_string(floatValue_);
  } else {
    size_t objectAddress = reinterpret_cast<std::size_t>(objectValue());
    std::stringstream id_stream;
    id_stream << std::hex << objectAddress;
    std::string id(id_stream.str());
    returnString += "<object-pointer id=\"" + id + "\"/>";
  }
  
  return returnString;
}



