#ifndef MAJOR_METHOD_MANAGER_H
#define MAJOR_METHOD_MANAGER_H

#include <vector>
#include <string>
#include <iostream>

class MethodDefinition;

class MethodManager {
private:
  //owned memory
  std::vector<MethodDefinition*> allMethods_;
public:
  MethodManager();
  
  //copy constructor
  MethodManager(const MethodManager &original);
  
  ~MethodManager();
  
  MethodDefinition* getMethod(std::string identifier);
  void addMethod(MethodDefinition* newMethod);
  std::vector<MethodDefinition*>* allMethods();

  //lists out all the methods in the method manager  
  void listMethods();
};

#endif
