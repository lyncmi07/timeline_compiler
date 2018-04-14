#ifndef MAJOR_VALUE_RETURNABLE_H
#define MAJOR_VALUE_RETURNABLE_H

class ReturnableValue;

//(interface)
/*
 * This interface is used by TokenObjects that need to return
 * a value that can be stored in a Variable.
 * The value is generated and returned by the returningCode() method.
 */
class ValueReturnable {
public:
  virtual ~ValueReturnable(){}
  
  virtual ReturnableValue* returningCode() = 0;
};

#endif
