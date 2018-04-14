#ifndef MAJOR_VALUE_MANAGER_H
#define MAJOR_VALUE_MANAGER_H

#include <vector>
#include <string>

class ReturnableValue;
class Object;

class ValueManager {
private:
  std::vector<ReturnableValue*> allValues_;
  bool dynamicValuesReady_;
public:
  ValueManager();
  ~ValueManager();
  
  ReturnableValue* newValue(std::string stringLiteralValue);
  ReturnableValue* newValue(int integerValue);
  ReturnableValue* newValue(float floatValue);
  ReturnableValue* newValue(std::string valueType, Object* objectValue);
  void newValue(ReturnableValue* customValue);
  
  /*
   * After the program execution has been finished, this is called in order to allow dynamic values
   * to be used in the upcoming timeline processing phase.
   */
  void allowDynamicValueUse();
  
  /*
   * Can be used to check if dynamic values are ready to be used.
   * This is helpful for when a dynamic value may need to be generated after the point at which that can be accessed
   */ 
  bool dynamicValuesCanBeUsed();
};

#endif
