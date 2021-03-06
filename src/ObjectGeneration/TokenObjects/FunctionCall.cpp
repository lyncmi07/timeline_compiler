#include "FunctionCall.h"
#include "ParameterSettingStatement.h"
#include "Expression.h"
#include "MethodDefinition.h"
#include "ConstructionDefinition.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Variables/ReturnableValue.h"
#include "../../Variables/MethodManager.h"
#include "../../Variables/CreatedClass.h"
#include "../../Variables/VariableManager.h"
#include "../../Variables/EventManager.h"
#include "../../Variables/Event.h"
#include "../../Variables/Variable.h"
#include "../../Variables/ClassDefinitionManager.h"
#include "../../Variables/ValueManager.h"
#include "../../Variables/DynamicReturnableValue.h"
#include "../../Helpers/StringFunctions.h"
#include "../../Exceptions/CompilerExceptions.h"
#include <iostream>

FunctionCall::FunctionCall(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, ClassDefinitionManager* classDefinitionManager, EventManager* eventManager, std::string identifier, ParameterSettingStatement* parameterSettingStatement) 
  : TokenObject(sourceFileIndex) {
  parameterSettingStatement_ = parameterSettingStatement;
  functionName_ = str_func::removeWhitespace(identifier);
  classDefinitionManager_ = classDefinitionManager;
  sourceFile_ = sourceFile;
  eventManager_ = eventManager;
  valueManager_ = valueManager;
}

FunctionCall::FunctionCall(int sourceFileIndex, PreProcessedFile* sourceFile, std::string identifier) 
  : TokenObject(sourceFileIndex) {
  functionName_ = str_func::removeWhitespace(identifier);
  sourceFile_ = sourceFile;
  parameterSettingStatement_ = nullptr;
  eventManager_ = nullptr;
}

FunctionCall::FunctionCall(const FunctionCall &original) 
 : TokenObject(original.sourceFileIndex_) {
  sourceFile_ = original.sourceFile_;
  eventManager_ = original.eventManager_;
  functionName_ = original.functionName_;
  if(original.parameterSettingStatement_ != nullptr) {
    parameterSettingStatement_ = new ParameterSettingStatement(*(original.parameterSettingStatement_));
  }
}

FunctionCall::~FunctionCall() {
  delete parameterSettingStatement_;
}

void FunctionCall::runCode() {
  //SPECIAL CASES
  if(functionName_.compare("print") == 0) {
    if(parameterSettingStatement_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m The print function takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    }
    if(parameterSettingStatement_->allParameters()->size() > 1) {
      std::cout << "\033[1;41mERROR:\033[0m The print function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      ReturnableValue* value = parameterSettingStatement_->allParameters()->at(0)->returningCode();
      if(value->valueType().compare("string") == 0) {
        std::cout << value->stringLiteralValue();
      } else if(value->valueType().compare("int") == 0) {
        std::cout << value->integerValue();
      } else if(value->valueType().compare("float") == 0) {
        std::cout << value->floatValue();
      } else if (value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0){
        std::cout << "\033[1;41mERROR:\033[0m dynamic values cannot be accessed during the execution of the program: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Cannot print Objects: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      }
    }
  } else if(functionName_.compare("println") == 0) {
    if(parameterSettingStatement_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m The println function takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    }
    if(parameterSettingStatement_->allParameters()->size() > 1) {
      std::cout << "\033[1;41mERROR:\033[0m The println function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      ReturnableValue* value = parameterSettingStatement_->allParameters()->at(0)->returningCode();
      if(value->valueType().compare("string") == 0) {
        std::cout << value->stringLiteralValue() << std::endl;
      } else if(value->valueType().compare("int") == 0) {
        std::cout << value->integerValue() << std::endl;
      } else if(value->valueType().compare("float") == 0) {
        std::cout << value->floatValue() << std::endl;
      } else if (value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0){
        std::cout << "\033[1;41mERROR:\033[0m dynamic values cannot be accessed during the execution of the program: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Cannot print Objects: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      }
    }
  } else {
    std::cout << "\033[1;41mERROR:\033[0m There is no function '" << functionName_ << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::MethodCallException();
  }
}
void FunctionCall::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  //SPECIAL CASES
  if(functionName_.compare("print") == 0) {
    if(parameterSettingStatement_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m  The print function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    }
    if(parameterSettingStatement_->allParameters()->size() > 1) {
      std::cout << "\033[1;41mERROR:\033[0m The print function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      ReturnableValue* value = parameterSettingStatement_->allParameters()->at(0)->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
      if(value->valueType().compare("string") == 0) {
        std::cout << value->stringLiteralValue();
      } else if(value->valueType().compare("int") == 0) {
        std::cout << value->integerValue();
      } else if(value->valueType().compare("float") == 0) {
        std::cout << value->floatValue();
      } else if (value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0){
        if(((DynamicReturnableValue*)value)->readyForUse()) {
          if(((DynamicReturnableValue*)value)->realValueType().compare("string") == 0) {
            std::cout << ((DynamicReturnableValue*)value)->stringLiteralValue() << std::endl;
          } else if(((DynamicReturnableValue*)value)->realValueType().compare("int") == 0) {
            std::cout << ((DynamicReturnableValue*)value)->integerValue() << std::endl;
          } else if(((DynamicReturnableValue*)value)->realValueType().compare("float") == 0) {
            std::cout << ((DynamicReturnableValue*)value)->floatValue() << std::endl;
          } else {
            std::cout << "\033[1;41mERROR:\033[0m Cannot print Objects: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
            throw comp_except::MethodCallException();
          }
        } else {
          std::cout << "\033[1;41mERROR:\033[0m dynamic values cannot be accessed during the execution of the program: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
          throw comp_except::StructureException();
        }
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Cannot print Objects: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      }
    }
  } else if(functionName_.compare("println") == 0) {
    if(parameterSettingStatement_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m  The println function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    }
    if(parameterSettingStatement_->allParameters()->size() > 1) {
      std::cout << "\033[1;41mERROR:\033[0m: The println function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      ReturnableValue* value = parameterSettingStatement_->allParameters()->at(0)->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
      if(value->valueType().compare("string") == 0) {
        std::cout << value->stringLiteralValue() << std::endl;
      } else if(value->valueType().compare("int") == 0) {
        std::cout << value->integerValue() << std::endl;
      } else if(value->valueType().compare("float") == 0) {
        std::cout << value->floatValue() << std::endl;
      } else if (value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0){
        if(((DynamicReturnableValue*)value)->readyForUse()) {
          if(((DynamicReturnableValue*)value)->realValueType().compare("string") == 0) {
            std::cout << ((DynamicReturnableValue*)value)->stringLiteralValue() << std::endl;
          } else if(((DynamicReturnableValue*)value)->realValueType().compare("int") == 0) {
            std::cout << ((DynamicReturnableValue*)value)->integerValue() << std::endl;
          } else if(((DynamicReturnableValue*)value)->realValueType().compare("float") == 0) {
            std::cout << ((DynamicReturnableValue*)value)->floatValue() << std::endl;
          } else {
            std::cout << "\033[1;41mERROR:\033[0m Cannot print Objects: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
            throw comp_except::MethodCallException();
          }
        } else {
          std::cout << "\033[1;41mERROR:\033[0m dynamic values cannot be accessed during the execution of the program: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
          throw comp_except::StructureException();
        }
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Cannot print Objects: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      }
    }
  } else if(functionName_.compare("super") == 0) {
    if(parentClass == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m Only the constructor can run the super method: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      ConstructionDefinition* parentConstructor = parentClass->constructionDefinition();
      if(parentConstructor == nullptr) {
        std::cout << "\033[1;41mERROR:\033[0m There is no constructor for the class '" << parentClass->className() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      } else {
        parentConstructor->runAsSuperCode(parameterSettingStatement_, classVariableManager, classMethodManager, parentClass);
      }
    }
  } else {
    //check for methods in this class to run
    MethodDefinition* runningMethod = classMethodManager->getMethod(functionName_);
    if(runningMethod == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m There is no method '" << functionName_ << "' in this class: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      if(runningMethod->returnType().compare("empty") != 0) {
        //TODO: what's this?
        //std::cout << "FunctionCall.cpp line 108. Not implemented yet" << std::endl;
        runningMethod->runAsReturningMethodInsideClass(parameterSettingStatement_, classVariableManager, classMethodManager, nullptr);
      } else {
        runningMethod->runAsVoidMethod(parameterSettingStatement_, classVariableManager, classMethodManager, nullptr);
      }
    }
  }
}

ReturnableValue* FunctionCall::returningCode() {
  //SPECIAL CASES
  if(functionName_.compare("GetEventContainingDataValue") == 0) {
    if(parameterSettingStatement_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m The GetEventContainingDataValue function takes three parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    }
    if(parameterSettingStatement_->allParameters()->size() == 3) {
      std::vector<ReturnableValue*> values;
      for(int i = 0; i < parameterSettingStatement_->allParameters()->size(); i++) {
        ReturnableValue* value = parameterSettingStatement_->allParameters()->at(i)->returningCode();
        values.push_back(value);
      }
      
      if(values[0]->valueType().compare("string") == 0 && values[1]->valueType().compare("string") == 0) {
        //from here a DynamicReturnableValue will be created that can be run at a later date once the timeline has been processed
        //the class of objects that will need to be searched through
        CreatedClass* givenClass = classDefinitionManager_->findClass(values[0]->stringLiteralValue());
        if(givenClass == nullptr) {
          std::cout << "\033[1;41mERROR:\033[0m There is no class '" << values[0]->stringLiteralValue() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
          throw comp_except::MethodCallException();
        } else {
          //now check that the sub variable being used actually exists in this classs:
          Variable* var = givenClass->subVariables()->getVariable(values[1]->stringLiteralValue());
          if(var == nullptr) {
            std::cout << "\033[1;41mERROR:\033[0m There is no variable '" << values[1]->stringLiteralValue() 
                      << "' in the class '" << values[0]->stringLiteralValue() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
            throw comp_except::VariableNotFoundException();
          } else {
            //everything checks out, therefore create a DynamicReturnableValue
            DynamicReturnableValue* value = new DynamicReturnableValue(eventManager_, givenClass, values[1]->stringLiteralValue(), values[2]);
            valueManager_->newValue(value);
            return value;
          }
        }
      } else {
        std::cout << "\033[1;41mERROR:\033[0m The GetEventContainingDataValue function takes three parameters of types (string, sting, any): " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      }
    } else {
      std::cout << "\033[1;41mERROR:\033[0m The GetEventContainingDataValue function takes three parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } 
  } else {
    std::cout << "\033[1;41mERROR:\033[0m There is no function '" << functionName_ << "' that returns a value: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::MethodCallException();
  }
}

ReturnableValue* FunctionCall::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  //SPECIAL CASES
  if(functionName_.compare("return") == 0) {
    if(parameterSettingStatement_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m  The return function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    }
    if(parameterSettingStatement_->allParameters()->size()  > 1) {
      std::cout << "\033[1;41mERROR:\033[0m The return function only takes one parameter: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      return parameterSettingStatement_->allParameters()->at(0)->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);    }
  } else if(functionName_.compare("GetEventContainingDataValue") == 0) {
    if(parameterSettingStatement_ == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m The GetEventContainingDataValue function takes three parameters of types (string, sting, any): " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
    }
    if(parameterSettingStatement_->allParameters()->size() == 3) {
      std::vector<ReturnableValue*> values;
      for(int i = 0; i < parameterSettingStatement_->allParameters()->size(); i++) {
        ReturnableValue* value = parameterSettingStatement_->allParameters()->at(i)->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
        values.push_back(value);
      }
      
      if(values[0]->valueType().compare("string") == 0 && values[1]->valueType().compare("string") == 0) {
      
        CreatedClass* givenClass = classDefinitionManager_->findClass(values[0]->stringLiteralValue());
      
        if(givenClass == nullptr) {
          std::cout << "\033[1;41mERROR:\033[0m There is no class '" << values[0]->stringLiteralValue() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
          throw comp_except::MethodCallException();
        }
        
        if(valueManager_->dynamicValuesCanBeUsed()) {
          for(int i = 0; i < eventManager_->allEvents()->size(); i++) {
            if(eventManager_->allEvents()->at(i)->givenClass()->className().compare(values[0]->stringLiteralValue()) == 0) {
              Variable* var = eventManager_->allEvents()->at(i)->variableManager()->getVariable(values[1]->stringLiteralValue());
              if(var == nullptr) {
                std::cout << "\033[1;41mERROR:\033[0m There is no variable '" << values[1]->stringLiteralValue() << "' in the class '"
                          << values[0]->stringLiteralValue() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
                throw comp_except::VariableNotFoundException();
              } else {
                //in correct place
                if(var->value() == values[2] && eventManager_->allEvents()->at(i)->happens()) {
                  ReturnableValue* givenEvent = new ReturnableValue(eventManager_->allEvents()->at(i)->givenClass()->className(),
                                                                    eventManager_->allEvents()->at(i));
                  valueManager_->newValue(givenEvent);
                  return givenEvent;
                }
              }
            }
          }
        } else {
          //not ready to find the actual value so currently use a dynamic value
          DynamicReturnableValue* value = new DynamicReturnableValue(eventManager_, givenClass, values[1]->stringLiteralValue(), values[2]);
          valueManager_->newValue(value);
          return value;
        }
      } else {
        std::cout << "\033[1;41mERROR:\033[0m The GetEventContainingDataValue function takes three parameters of types (string, sting, any): " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      }
    }
  } else {
    MethodDefinition* runningMethod = classMethodManager->getMethod(functionName_);
    if(runningMethod == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m There is no method '" << functionName_ << "' in this class: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      return runningMethod->runAsReturningMethodInsideClass(parameterSettingStatement_, classVariableManager, classMethodManager, nullptr);
    }
    return nullptr;
  }
}

ReturnableValue* FunctionCall::returningCodeSeparateClassMode(VariableManager* separateClassVariableManager,
                                                              VariableManager* classVariableManager,
                                                              MethodManager* classMethodManager,
                                                              CreatedClass* parentClass) {
  //SPECIAL CASES
  if(functionName_.compare("return") == 0) {
    std::cout << "\033[1;41mERROR:\033[0m The return function can only be used in class methods: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::MethodCallException();
  } else {
    MethodDefinition* runningMethod = classMethodManager->getMethod(functionName_);
    if(runningMethod == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m There is no method '" << functionName_ << "' in this class: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      return runningMethod->runAsReturningMethodInSeparateClass(parameterSettingStatement_, separateClassVariableManager, classVariableManager, classMethodManager, nullptr);
    }
  }
}

ReturnableValue* FunctionCall::returningCodeOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  //SPECIAL CASES
  if(functionName_.compare("return") == 0) {
    std::cout << "\033[1;41mERROR:\033[0m The return function can only be used in class methods: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
    throw comp_except::MethodCallException();
  } else {
    MethodDefinition* runningMethod = classMethodManager->getMethod(functionName_);
    if(runningMethod == nullptr) {
      std::cout << "\033[1;41mERROR:\033[0m There is no method '" << functionName_ << "' in this class: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      return runningMethod->runAsReturningMethod(parameterSettingStatement_, classVariableManager, classMethodManager, nullptr);
    }
  }
}

std::string FunctionCall::functionName() {
  return functionName_;
}

ParameterSettingStatement* FunctionCall::parameterSettingStatement() {
  return parameterSettingStatement_;
}

bool FunctionCall::immediateRun() {
  if(functionName_.compare("GetEventContainingDataValue") == 0) {
    return false;
  } else {
    return true;
  }
}

