#include "MethodDefinition.h"
#include "ParameterDefinitionStatement.h"
#include "ParameterSettingStatement.h"
#include "CodeBlock.h"
#include "Expression.h"
#include "VariableInitiation.h"
#include "../../Variables/ReturnableValue.h"
#include "../../Variables/DynamicReturnableValue.h"
#include "../../Variables/VariableManager.h"
#include "../../Variables/Variable.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Helpers/StringFunctions.h"
#include "../../Exceptions/CompilerExceptions.h"
#include <iostream>

void MethodDefinition::convertParametersToVariables(ParameterSettingStatement* parameterInputs, VariableManager* localVariableManager) {
  if(parameterInputs == nullptr) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  if(parameterInputs->allParameters()->size() != parameterDefinitionStatement_->allParameters()->size()) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  for(int i = 0; i < parameterInputs->allParameters()->size(); i++){
    ReturnableValue* value = parameterInputs->allParameters()->at(i)->returningCode();
    //get what type the value should be
    std::string parameterType = parameterDefinitionStatement_->allParameters()->at(i)->identifier1();    
    if(value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0) {
      if(((DynamicReturnableValue*)value)->realValueType().compare(parameterType) != 0) {
        //type mismatch
        std::cout << "\033[1;41mERROR:\033[0m Parameter " << i << " type mismatch. Should be type '" << parameterType << "' not '" << ((DynamicReturnableValue*)value)->realValueType() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      } else {
        //value types do match therefore generate variable
        localVariableManager->addVariable(parameterDefinitionStatement_->allParameters()->at(i)->identifier1(),
                                          parameterDefinitionStatement_->allParameters()->at(i)->identifier2(),
                                          parameterInputs->allParameters()->at(i));
      }
    } else if(value->valueType().compare(parameterType) != 0) {
      //type mismatch
      std::cout << "\033[1;41mERROR:\033[0m Parameter " << i << " type mismatch. Should be type '" << parameterType << "' not '" << value->valueType() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      //value types do match therefore generate variable
      localVariableManager->addVariable(parameterDefinitionStatement_->allParameters()->at(i)->identifier1(),
                                        parameterDefinitionStatement_->allParameters()->at(i)->identifier2(),
                                        parameterInputs->allParameters()->at(i));
    }
  }
}

void MethodDefinition::convertParametersToVariablesInsideClass(ParameterSettingStatement* parameterInputs,
                                                                VariableManager* localVariableManager,
                                                                VariableManager* classVariableManager,
                                                                MethodManager* classMethodManager) {
  if(parameterInputs == nullptr) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  if(parameterInputs->allParameters()->size() != parameterDefinitionStatement_->allParameters()->size()) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  for(int i = 0; i < parameterInputs->allParameters()->size(); i++){
    ReturnableValue* value = parameterInputs->allParameters()->at(i)->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
    //get what type the value should be
    std::string parameterType = parameterDefinitionStatement_->allParameters()->at(i)->identifier1();    
    
    if(value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0) {
      if(((DynamicReturnableValue*)value)->realValueType().compare(parameterType) != 0) {
        //type mismatch
        std::cout << "\033[1;41mERROR:\033[0m Parameter " << i << " type mismatch. Should be type '" << parameterType << "' not '" << ((DynamicReturnableValue*)value)->realValueType() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      } else {
        //value types do match therefore generate variable
        localVariableManager->addVariable(parameterDefinitionStatement_->allParameters()->at(i)->identifier1(),
                                          parameterDefinitionStatement_->allParameters()->at(i)->identifier2(),
                                          parameterInputs->allParameters()->at(i));
      }
    } else if(value->valueType().compare(parameterType) != 0) {
      //type mismatch
      std::cout << "\033[1;41mERROR:\033[0m Parameter " << i << " type mismatch. Should be type '" << parameterType << "' not '" << value->valueType() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      //value types do match therefore generate variable
      Variable* newVar = localVariableManager->addVariable(parameterDefinitionStatement_->allParameters()->at(i)->identifier1(),
                                        parameterDefinitionStatement_->allParameters()->at(i)->identifier2());
      newVar->setValueInsideClass(parameterInputs->allParameters()->at(i), classVariableManager, classMethodManager, nullptr);
    }
  }
}

MethodDefinition::MethodDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, std::string identifier1, std::string identifier2, ParameterDefinitionStatement* parameterDefinitionStatement, CodeBlock* codeBlock)
  : TokenObject(sourceFileIndex) {
  returnType_ = str_func::removeWhitespace(identifier1);
  methodIdentifier_ = str_func::removeWhitespace(identifier2);
  parameterDefinitionStatement_ = parameterDefinitionStatement;
  codeBlock_ = codeBlock; 
  sourceFile_ = sourceFile;
}

MethodDefinition::MethodDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, std::string identifier1, std::string identifier2, ParameterDefinitionStatement* parameterDefinitionStatement)
  : TokenObject(sourceFileIndex) {
  returnType_ = identifier1;
  methodIdentifier_ = identifier2;
  parameterDefinitionStatement_ = parameterDefinitionStatement;
  codeBlock_ = nullptr;
  sourceFile_ = sourceFile;
}

MethodDefinition::MethodDefinition(const MethodDefinition &original) 
 : TokenObject(original.sourceFileIndex_) {
 
  returnType_ = original.returnType_;
  methodIdentifier_ = original.methodIdentifier_;
  
  if(original.parameterDefinitionStatement_ != nullptr) {
    parameterDefinitionStatement_ = new ParameterDefinitionStatement(*(original.parameterDefinitionStatement_));
  }
  if(original.codeBlock_ != nullptr) {
    codeBlock_ = new CodeBlock(*(original.codeBlock_));
  } else {
    codeBlock_ = nullptr;
  }
  
  sourceFile_ = original.sourceFile_;
}

MethodDefinition::~MethodDefinition() {
  delete parameterDefinitionStatement_;
  delete codeBlock_;
}

std::string MethodDefinition::returnType() {
  return returnType_;
}

std::string MethodDefinition::methodIdentifier() {
  return methodIdentifier_;
}

ParameterDefinitionStatement* MethodDefinition::parameterDefinitionStatement() {
  return parameterDefinitionStatement_;
}

CodeBlock* MethodDefinition::codeBlock() {
  return codeBlock_;
} 

void MethodDefinition::runAsVoidMethod(ParameterSettingStatement* methodInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  VariableManager* localManager = new VariableManager();
  
  if(methodInputs != nullptr) {
    convertParametersToVariables(methodInputs, localManager);
  } else if(parameterDefinitionStatement_->allParameters()->size() > 0) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  
  classVariableManager->addLocalVariableManager(localManager);
  
  if(codeBlock_ == nullptr) {
    return;
  }
  
  codeBlock_->runCodeClassMode(classVariableManager, classMethodManager, nullptr);
  classVariableManager->removeLocalVariableManager();
  
  //TODO: find a way to delete managers without destroying the values of the variables that may have been accessed
  //delete localManager;
}

ReturnableValue* MethodDefinition::runAsReturningMethod(ParameterSettingStatement* methodInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  /*if(returnType_.compare("empty") == 0) {
    return nullptr;
  }*/
  VariableManager* localManager = new VariableManager();
  if(methodInputs != nullptr) {
    convertParametersToVariables(methodInputs, localManager);
  } else if(parameterDefinitionStatement_->allParameters()->size() > 0) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  
  
  classVariableManager->addLocalVariableManager(localManager);
  
  ReturnableValue* value = nullptr;
  
  if(codeBlock_ != nullptr) {
    value = codeBlock_->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
  }
  
  classVariableManager->removeLocalVariableManager();
  
  //TODO: find a way to delete managers without destroying the values of the variables that may have been accessed
  //pretty sure this is ok now
  //delete localManager;
  
  return value;
}

ReturnableValue* MethodDefinition::runAsReturningMethodInSeparateClass(ParameterSettingStatement* methodInputs,
                                                                       VariableManager* separateClassVariableManager,
                                                                       VariableManager* classVariableManager,
                                                                       MethodManager* classMethodManager,
                                                                       CreatedClass* parentClass) {
  if(returnType_.compare("empty") == 0) {
    return nullptr;
  }
  
  VariableManager* localManager = new VariableManager();
  if(methodInputs != nullptr) {
    convertParametersToVariablesInsideClass(methodInputs, localManager, separateClassVariableManager, classMethodManager);
  } else if(parameterDefinitionStatement_->allParameters()->size() > 0) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  
  classVariableManager->addLocalVariableManager(localManager);
  
  ReturnableValue* value = nullptr;
  
  if(codeBlock_ != nullptr) {
    value = codeBlock_->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
  }
  
  classVariableManager->removeLocalVariableManager();
  
  return value;
  
}

void MethodDefinition::runAsVoidMethodInsideClass(ParameterSettingStatement* methodInputs,
                                                  VariableManager* classVariableManager,
                                                  MethodManager*  classMethodManager,
                                                  CreatedClass* parentClass) {
  VariableManager* localManager = new VariableManager();
  
  if(methodInputs != nullptr) {
    convertParametersToVariablesInsideClass(methodInputs, localManager, classVariableManager, classMethodManager);
  } else if(parameterDefinitionStatement_->allParameters()->size() > 0) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  
  classVariableManager->addLocalVariableManager(localManager);
  
  if(codeBlock_ != nullptr) {
    codeBlock_->runCodeClassMode(classVariableManager, classMethodManager, nullptr);
  }
  
  classVariableManager->removeLocalVariableManager();
  
  
  //TODO: find a way to delete managers without destroying the values of the variables that may have been accessed
  //pretty sure this is ok now
  //delete localManager;
}

ReturnableValue* MethodDefinition::runAsReturningMethodInsideClass(ParameterSettingStatement* methodInputs,
                                                  VariableManager* classVariableManager,
                                                  MethodManager* classMethodManager,
                                                  CreatedClass* parentClass) {
  if(returnType_.compare("empty") == 0) {
    return nullptr;
  }
  VariableManager* localManager = new VariableManager();
  
  if(methodInputs != nullptr) {
    convertParametersToVariablesInsideClass(methodInputs, localManager, classVariableManager, classMethodManager);
  } else if(parameterDefinitionStatement_->allParameters()->size() > 0) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  
  
  classVariableManager->addLocalVariableManager(localManager);
  
  if(codeBlock_ == nullptr) {
    return nullptr;
  }
  
  ReturnableValue* value = codeBlock_->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
  classVariableManager->removeLocalVariableManager();
  
  //TODO: find a way to delete managers without destroying the values of the variables that may have been accessed
  //pretty sure this is ok now
  //delete localManager;
  
  return value;
}

