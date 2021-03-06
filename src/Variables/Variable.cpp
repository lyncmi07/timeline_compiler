#include "Variable.h"
#include "ReturnableValue.h"
#include "DynamicReturnableValue.h"
#include "../ObjectGeneration/TokenObjects/Expression.h"
#include "VariableManager.h"
#include "MethodManager.h"
#include "CreatedClass.h"
#include <iostream>

Variable::Variable(std::string type, std::string identifier) {
  type_ = type;
  identifier_ = identifier;
  value_ = nullptr;
  valueSet_ = false;
}

Variable::~Variable() {
  //delete value_;
}

void Variable::setValue(Expression* valueGenerator) {
  valueSet_ = true;
  
  bool immediateRun_ = valueGenerator->immediateRun();
  inClass_ = false;
  if(immediateRun_ || !immediateRun_) {
    //generate the value and put it straight into the variable
    value_ = valueGenerator->returningCode();
    if(value_ != nullptr) {
      if(value_->valueType().compare("dynamic") == 0) {
        if(((DynamicReturnableValue*)value_)->realValueType().compare(type_) != 0) {
          std::cout << "\033[1;41mERROR:\033[0m Setting variable '" << identifier_ << "': Variable and Value types do not match." << std::endl;
          std::cout << type_ << " and " << ((DynamicReturnableValue*)value_)->realValueType() << " respectively" << std::endl;
          value_ = nullptr;
          valueSet_ = false;
        }
      } else if(value_->valueType().compare("dynamic_svi") == 0) {
        //assume correct type for now
        
      } else if(value_->valueType().compare(type_) != 0) {
        std::cout << "\033[1;41mERROR:\033[0m Setting variable '" << identifier_ << "': Variable and Value types do not match." << std::endl; 
        std::cout << type_ << " and " << value_->valueType() << " respectively" << std::endl;
        //this might do unwanted deletion of other variables
        //DON'T LET IT DO THIS
        //delete value_;
        value_ = nullptr;
        valueSet_ = false;
      }
    }
  } else {
    //valueGenerator_ = valueGenerator;
//    valueSet_ = false;
    //value_ = new DynamicReturnableValue(valueGenerator);
  }
}

void Variable::setValue(ReturnableValue* actualValue) {
  value_ = actualValue;
  valueSet_ = true;
}

void Variable::setValueInsideClass(Expression* valueGenerator, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  valueSet_ = true;
  
  classVariableManager_ = classVariableManager;
  classMethodManager_ = classMethodManager;
  inClass_ = true;
  bool immediateRun_ = valueGenerator->immediateRun();
  
  if(immediateRun_ || !immediateRun_) {
    //generate the value and put it straight into the variable;
    value_ = valueGenerator->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
    
    if(value_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m Cannot set variable to value that does not exist" << std::endl;
      value_ = nullptr;
      valueSet_ = false;
      return;
    }
    
    if(value_->valueType().compare("dynamic") == 0 || value_->valueType().compare("dynamic_svi") == 0) {
      if(((DynamicReturnableValue*)value_)->readyForUse()) {
        ((DynamicReturnableValue*)value_)->updateValue();
        if(((DynamicReturnableValue*)value_)->realValueType().compare(type_) != 0) {
          std::cout << "\033[1;41mERROR:\033[0m Setting variable '" << identifier_ << "': Variable and Value types do not match." << std::endl;
          std::cout << type_ << " and " << ((DynamicReturnableValue*)value_)->realValueType() << " respectively" << std::endl;
          value_ = nullptr;
          valueSet_ = false;
        }
      } else {
        if(((DynamicReturnableValue*)value_)->realValueType().compare(type_) != 0) {
          std::cout << "\033[1;41mERROR:\033[0m Setting variable '" << identifier_ << "': Variable and Value types do not match." << std::endl;
          std::cout << type_ << " and " << ((DynamicReturnableValue*)value_)->realValueType() << " respectively" << std::endl;
          value_ = nullptr;
          valueSet_ = false;
        }
      }
    }  else if(value_->valueType().compare(type_) != 0) {
      std::cout << "\033[1;41mERROR:\033[0m Setting variable '" << identifier_ << "': Variable and Value types do not match." << std::endl;
      std::cout << type_ << " and " << value_->valueType() << " respectively" << std::endl;
      
      //this might do unwanted deletion of other variables
      value_ = nullptr;
      valueSet_ = false;
    }
  } else {
  
  }
}

ReturnableValue* Variable::value() {
  return value_;
}

std::string Variable::variableType() {
  return type_;
}

std::string Variable::identifier() {
  return identifier_;
}


std::string Variable::toString() {
  if(valueSet_) {
    return type_ + " " + identifier_ + " = " + value_->toString();
  } else {
    return type_ + " " + identifier_ + " = NULL";
  }
}

std::string Variable::printToXML(int spaceIndent) {
  std::string returnString = "";
  std::string indent(spaceIndent,' ');
  
  returnString += indent + "<var type=\"" + type_ + "\" name=\"" + identifier_ + "\">";
  if(valueSet_) {
    returnString += value_->printToXML(spaceIndent+2);
  } else {
    returnString += "NULL";
  }
  returnString += "</var>\n";
  
  return returnString;
}
