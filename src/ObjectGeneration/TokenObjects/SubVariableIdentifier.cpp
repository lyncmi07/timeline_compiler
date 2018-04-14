#include "SubVariableIdentifier.h"
#include "PossibleObject.h"
#include "../../Variables/ValueManager.h"
#include "../../Variables/DynamicReturnableValue.h"
#include "../../Exceptions/CompilerExceptions.h"
#include <iostream>

SubVariableIdentifierType SubVariableIdentifier::type() {
  return type_;
}

SubVariableIdentifier::SubVariableIdentifier(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, PossibleObject* possibleObject1, PossibleObject* possibleObject2)
  : TokenObject(sourceFileIndex) {
  type_ = SVI_PP;
  possibleObject1_ = possibleObject1;
  possibleObject2_ = possibleObject2;
  possibleObject_ = nullptr;
  subVariableIdentifier_ = nullptr;
  sourceFile_ = sourceFile;
  valueManager_ = valueManager;
}

SubVariableIdentifier::SubVariableIdentifier(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, PossibleObject* possibleObject, SubVariableIdentifier* subVariableIdentifier)
  : TokenObject(sourceFileIndex) {
  type_ = SVI_PS;
  possibleObject_ = possibleObject;
  subVariableIdentifier_ = subVariableIdentifier;
  possibleObject1_ = nullptr;
  possibleObject2_ = nullptr;
  sourceFile_ = sourceFile;
  valueManager_ = valueManager;
}

SubVariableIdentifier::SubVariableIdentifier(const SubVariableIdentifier &original) 
 : TokenObject(original.sourceFileIndex_) {
  type_ = original.type_;
  switch(type_) {
  case SVI_PP:
    possibleObject1_ = new PossibleObject(*original.possibleObject1_);
    possibleObject2_ = new PossibleObject(*original.possibleObject2_);
    possibleObject_ = nullptr;
    subVariableIdentifier_ = nullptr;
    break;
  case SVI_PS:
    possibleObject1_ = nullptr;
    possibleObject2_ = nullptr;
    possibleObject_ = new PossibleObject(*original.possibleObject_);
    subVariableIdentifier_ = new SubVariableIdentifier(*original.subVariableIdentifier_);
  }
}

SubVariableIdentifier::~SubVariableIdentifier() {
  delete possibleObject1_;
  delete possibleObject2_;
  delete possibleObject_;
  delete subVariableIdentifier_;
}

void SubVariableIdentifier::runCode() {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCode();
    
    if(value->isObject()) {
      possibleObject2_->returningCodeOutsideClassMode(value->objectValue()->variableManager(), 
                                               value->objectValue()->givenClass()->methods(),
                                               value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCode();
    
    if(value->isObject()) {
      //TODO: FIX THIS WE NEED AN OUTSIDE CLASS
      subVariableIdentifier_->returningCodeOutsideClassMode(value->objectValue()->variableManager(),
                                                     value->objectValue()->givenClass()->methods(),
                                                     value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}

void SubVariableIdentifier::runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
}

ReturnableValue* SubVariableIdentifier::returningCode() {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCode();
    //TODO: if you find that this is a DynamicReturnableValue then this method should also return a DynamicReturnableValue
    if(value->valueType().compare("dynamic") == 0) {
      DynamicReturnableValue* returnValue = new DynamicReturnableValue((DynamicReturnableValue*)value, possibleObject2_);
      valueManager_->newValue(returnValue);
      return returnValue;  
    } else if(value->valueType().compare("dynamic_svi") == 0) {
      std::cout << "\033[1;41mERROR:\033[0m Sub objects of dynamic values cannot be accessed during program execution: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::StructureException();
    }
    if(value->isObject()) {
      return possibleObject2_->returningCodeOutsideClassMode(value->objectValue()->variableManager(), 
                                                      value->objectValue()->givenClass()->methods(),
                                                      value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
      return nullptr;
    }
    
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCode();
    if(value->valueType().compare("dynamic") == 0) {
      DynamicReturnableValue* returnValue = new DynamicReturnableValue((DynamicReturnableValue*)value, subVariableIdentifier_);
      valueManager_->newValue(returnValue);
      return returnValue;  
    } else if(value->valueType().compare("dynamic_svi") == 0) {
      std::cout << "\033[1;41mERROR:\033[0m Sub objects of dynamic values cannot be accessed during program execution: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::StructureException();
    }
    if(value->isObject()) {
      //TODO: FIX THIS WE NEED AN OUTSIDE CLASS
      return subVariableIdentifier_->returningCodeOutsideClassMode(value->objectValue()->variableManager(),
                                                            value->objectValue()->givenClass()->methods(),
                                                            value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}

Variable* SubVariableIdentifier::getVariable() {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCode();
    
    if(value->isObject()) {
      return possibleObject2_->getVariable(value->objectValue()->variableManager());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCode();
    
    if(value->isObject()) {
      return subVariableIdentifier_->getVariableOutsideClassMode(value->objectValue()->variableManager(),
                                                          value->objectValue()->givenClass()->methods(),
                                                          value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}

Variable* SubVariableIdentifier::getVariableClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    if(value->valueType().compare("dynamic_svi") == 0 || value->valueType().compare("dynamic") == 0) {
      //std::cout << "definetly in the correct location" << std::endl;
      if(((DynamicReturnableValue*)value)->readyForUse()) {
        //TODO: check for if it is some other kind of variable. in this case we know it is a particular type of object
        ((DynamicReturnableValue*)value)->updateValue();
        Object* givenEvent = value->objectValue();
        if(((DynamicReturnableValue*)value)->realValueType().compare("string") == 0 ||
           ((DynamicReturnableValue*)value)->realValueType().compare("int") == 0  ||
           ((DynamicReturnableValue*)value)->realValueType().compare("float") == 0) {
          std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
          std::cout << "This is probably caused by a type mismatch." << std::endl;
          throw comp_except::VariableNotFoundException();
        }
        return possibleObject2_->getVariable(givenEvent->variableManager());
      } else {
        std::cout << "\033[1;41mERROR:\033[0m dynamic values cannot be accessed during the execution of the program: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
    }
    if(value->isObject()) {
      return possibleObject2_->getVariable(value->objectValue()->variableManager());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    
    if(value->valueType().compare("dynamic_svi") == 0 || value->valueType().compare("dynamic") == 0) {
      if(((DynamicReturnableValue*)value)->readyForUse()) {
        //TODO: check for if it is some other kind of variable. in this case we know it is a particular type of object
        ((DynamicReturnableValue*)value)->updateValue();
        Object* givenEvent = value->objectValue();
        if(((DynamicReturnableValue*)value)->realValueType().compare("string") == 0 ||
           ((DynamicReturnableValue*)value)->realValueType().compare("int") == 0  ||
           ((DynamicReturnableValue*)value)->realValueType().compare("float") == 0) {
          std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
          std::cout << "This is probably caused by a type mismatch." << std::endl;
          throw comp_except::VariableNotFoundException();
        }
        return subVariableIdentifier_->getVariableSeparateClassMode(classVariableManager,
                                                                  givenEvent->variableManager(),
                                                                  givenEvent->givenClass()->methods(),
                                                                  givenEvent->givenClass()->parentClass());
      } else {
        std::cout << "\033[1;41mERROR:\033[0m dynamic values cannot be accessed during the execution of the program: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::StructureException();
      }
    }
    
    if(value->isObject()) {
      return subVariableIdentifier_->getVariableSeparateClassMode(classVariableManager,
                                                                  value->objectValue()->variableManager(),
                                                                  value->objectValue()->givenClass()->methods(),
                                                                  value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}

Variable* SubVariableIdentifier::getVariableOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCodeOutsideClassMode(classVariableManager, classMethodManager, parentClass);
    
    if(value->isObject()) {
      return possibleObject2_->getVariable(value->objectValue()->variableManager());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCodeOutsideClassMode(classVariableManager, classMethodManager, parentClass);
    if(value->isObject()) {
      return subVariableIdentifier_->getVariableOutsideClassMode(value->objectValue()->variableManager(),
                                                                 value->objectValue()->givenClass()->methods(),
                                                                 value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}

Variable* SubVariableIdentifier::getVariableSeparateClassMode(VariableManager* separateClassVariableManager,
                                                              VariableManager* classVariableManager,
                                                              MethodManager* classMethodManager,
                                                              CreatedClass* parentClass) {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCodeSeparateClassMode(separateClassVariableManager,
                                                                              classVariableManager,
                                                                              classMethodManager,
                                                                              parentClass);
    if(value->isObject()) {
      return possibleObject2_->getVariable(value->objectValue()->variableManager());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCodeSeparateClassMode(separateClassVariableManager,
                                                                              classVariableManager,
                                                                              classMethodManager,
                                                                              parentClass);
                                                                              
    if(value->isObject()) {
      return subVariableIdentifier_->getVariableSeparateClassMode(separateClassVariableManager,
                                                                 value->objectValue()->variableManager(),
                                                                 value->objectValue()->givenClass()->methods(),
                                                                 value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}

ReturnableValue* SubVariableIdentifier::returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    
    if(value->valueType().compare("dynamic") == 0) {
      DynamicReturnableValue* returnValue = new DynamicReturnableValue((DynamicReturnableValue*)value, possibleObject2_);
      valueManager_->newValue(returnValue);
      return returnValue;  
    } else if(value->valueType().compare("dynamic_svi") == 0) {
      std::cout << "\033[1;41mERROR:\033[0m Sub objects of dynamic values cannot be accessed during program execution: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::StructureException();
    } 
    
    if(value->isObject()) {
      return possibleObject2_->returningCodeSeparateClassMode(classVariableManager,
                                                      value->objectValue()->variableManager(), 
                                                      value->objectValue()->givenClass()->methods(),
                                                      value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
    
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCodeClassMode(classVariableManager, classMethodManager, parentClass);
    
    if(value->isObject()) {
      return subVariableIdentifier_->returningCodeSeparateClassMode(classVariableManager,
                                                                    value->objectValue()->variableManager(),
                                                                    value->objectValue()->givenClass()->methods(),
                                                                    value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}

ReturnableValue* SubVariableIdentifier::returningCodeSeparateClassMode(VariableManager* separateClassVariableManager,
                                                                       VariableManager* classVariableManager,
                                                                       MethodManager* classMethodManager,
                                                                       CreatedClass* parentClass) {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCodeSeparateClassMode(separateClassVariableManager,
                                                                              classVariableManager, 
                                                                              classMethodManager, 
                                                                              parentClass);
    
    if(value->isObject()) {
      return possibleObject2_->returningCodeSeparateClassMode(separateClassVariableManager,
                                                              value->objectValue()->variableManager(), 
                                                              value->objectValue()->givenClass()->methods(),
                                                              value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  } else {
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCodeSeparateClassMode(separateClassVariableManager,
                                                                       classVariableManager, 
                                                                       classMethodManager, 
                                                                       parentClass);
                                                                       
    if(value->isObject()) {
      return subVariableIdentifier_->returningCodeSeparateClassMode(separateClassVariableManager,
                                                              value->objectValue()->variableManager(), 
                                                              value->objectValue()->givenClass()->methods(),
                                                              value->objectValue()->givenClass()->parentClass());
    }
  }
}

ReturnableValue* SubVariableIdentifier::returningCodeOutsideClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  if(type_ == SVI_PP) {
    ReturnableValue* value = possibleObject1_->returningCodeOutsideClassMode(classVariableManager, classMethodManager, parentClass);
    
    if(value->isObject()) {
      return possibleObject2_->returningCodeOutsideClassMode(value->objectValue()->variableManager(), 
                                                      value->objectValue()->givenClass()->methods(),
                                                      value->objectValue()->givenClass()->parentClass());                                                       
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  } else {  
    //type_ = SVI_PS
    ReturnableValue* value = possibleObject_->returningCodeOutsideClassMode(classVariableManager, classMethodManager, parentClass);
    
    if(value->isObject()) {
      return subVariableIdentifier_->returningCodeOutsideClassMode(value->objectValue()->variableManager(),
                                                            value->objectValue()->givenClass()->methods(),
                                                            value->objectValue()->givenClass()->parentClass());
    } else {
      std::cout << "\033[1;41mERROR:\033[0m There are no sub variables: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::VariableNotFoundException();
    }
  }
}



