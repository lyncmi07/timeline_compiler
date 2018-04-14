#ifndef MAJOR_RETURNABLE_VALUE_H
#define MAJOR_RETURNABLE_VALUE_H

#include <string>

class Object;
class Variable;

class ReturnableValue {
private:
  std::string valueType_;
  std::string stringLiteralValue_;
  int integerValue_;
  float floatValue_;
  
  bool isObject_;
  
  //not owned (owned by Event/Entity Manager)
  Object* objectValue_;
  
public:
  ReturnableValue(std::string stringLiteralValue);
  ReturnableValue(int integerValue);
  ReturnableValue(float floatValue);
  ReturnableValue(std::string valueType, Object* objectValue);
  
  ~ReturnableValue();
  
  virtual std::string stringLiteralValue();
  virtual int integerValue();
  virtual float floatValue();
  virtual Object* objectValue();
  
  std::string valueType();
  
  virtual std::string toString();
  
  virtual bool isObject();
  
  bool operator==(const ReturnableValue &comparison) const;
  
  virtual std::string printToXML(int spaceIndent);
};

#endif
