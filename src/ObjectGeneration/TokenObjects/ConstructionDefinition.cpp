#include "ConstructionDefinition.h"

#include <iostream>
#include "ParameterDefinitionStatement.h"
#include "ParameterSettingStatement.h"
#include "Expression.h"
#include "CodeBlock.h"
#include "VariableInitiation.h"
#include "../../Variables/VariableManager.h"
#include "../../Variables/ReturnableValue.h"
#include "../../Variables/DynamicReturnableValue.h"
#include "../../Variables/Variable.h"
#include "../../Parser/PreProcessedFile.h"
#include "../../Exceptions/CompilerExceptions.h"

void ConstructionDefinition::convertParametersToVariables(ParameterSettingStatement* parameterInputs, VariableManager* localVariableManager) {
  if(parameterInputs == nullptr) {
    std::cout << "\033[1;41mERROR:\033[0m Class takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters in constructor: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  if(parameterInputs->allParameters()->size() != parameterDefinitionStatement_->allParameters()->size()) {
    std::cout << "\033[1;41mERROR:\033[0m Class takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters in constructor: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  for(int i = 0; i < parameterInputs->allParameters()->size(); i++){
    ReturnableValue* value = parameterInputs->allParameters()->at(i)->returningCode();
    //get what type the value should be
    std::string parameterType = parameterDefinitionStatement_->allParameters()->at(i)->identifier1();    
    if(value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0) {
      if(((DynamicReturnableValue*)value)->realValueType().compare(parameterType) != 0) {
        //type mismatch
        std::cout << "\033[1;41mERROR:\033[0m Parameter " << i << " type mismatch. Should be type '" << parameterType << "' not '" << value->valueType() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      } else {
        //types are correct
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

void ConstructionDefinition::convertParametersToVariablesSuperMode(ParameterSettingStatement* parameterInputs,
                                                                   VariableManager* localVariableManager,
                                                                   VariableManager* classVariableManager,
                                                                   MethodManager* classMethodManager) {
  if(parameterInputs == nullptr) {
    std::cout << "\033[1;41mERROR:\033[0m Class takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters in constructor: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  if(parameterInputs->allParameters()->size() != parameterDefinitionStatement_->allParameters()->size()) {
    std::cout << "\033[1;41mERROR:\033[0m Class takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters in constructor: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  for(int i = 0; i < parameterInputs->allParameters()->size(); i++){
    ReturnableValue* value = parameterInputs->allParameters()->at(i)->returningCodeClassMode(classVariableManager, classMethodManager, nullptr);
    //get what type the value should be
    std::string parameterType = parameterDefinitionStatement_->allParameters()->at(i)->identifier1();   
    
    
    if(value->valueType().compare("dynamic") == 0 || value->valueType().compare("dynamic_svi") == 0) {
      if(((DynamicReturnableValue*)value)->realValueType().compare(parameterType) != 0) {
        //type mismatch
        std::cout << "\033[1;41mERROR:\033[0m Parameter " << i << " type mismatch. Should be type '" << parameterType << "' not '" << value->valueType() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
        throw comp_except::MethodCallException();
      } else {
        Variable* newVar = localVariableManager->addVariable(parameterDefinitionStatement_->allParameters()->at(i)->identifier1(),
                                                           parameterDefinitionStatement_->allParameters()->at(i)->identifier2());
        newVar->setValueInsideClass(parameterInputs->allParameters()->at(i), classVariableManager, classMethodManager, nullptr);
      }
    } else if(value->valueType().compare(parameterType) != 0) {
      //type mismatch
      std::cout << "\033[1;41mERROR:\033[0m Parameter " << i << " type mismatch. Should be type '" << parameterType << "' not '" << value->valueType() << "': " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl;
      throw comp_except::MethodCallException();
    } else {
      Variable* newVar = localVariableManager->addVariable(parameterDefinitionStatement_->allParameters()->at(i)->identifier1(),
                                                           parameterDefinitionStatement_->allParameters()->at(i)->identifier2());
      newVar->setValueInsideClass(parameterInputs->allParameters()->at(i), classVariableManager, classMethodManager, nullptr);
    }
  }
}

ConstructionDefinition::ConstructionDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, ParameterDefinitionStatement* parameterDefinitionStatement, CodeBlock* codeBlock)
  : TokenObject(sourceFileIndex) {
  parameterDefinitionStatement_ = parameterDefinitionStatement;
  codeBlock_ = codeBlock; 
  sourceFile_ = sourceFile;
}

ConstructionDefinition::ConstructionDefinition(int sourceFileIndex, PreProcessedFile* sourceFile, ParameterDefinitionStatement* parameterDefinitionStatement)
  : TokenObject(sourceFileIndex) {
  parameterDefinitionStatement_ = parameterDefinitionStatement;
  codeBlock_ = nullptr;
  sourceFile_ = sourceFile;
}

ConstructionDefinition::~ConstructionDefinition() {
  delete parameterDefinitionStatement_;
  delete codeBlock_;
}

void ConstructionDefinition::runAsConstructionCode(ParameterSettingStatement* constructionInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  VariableManager* localManager = new VariableManager();
  
  if(constructionInputs != nullptr) {
    convertParametersToVariables(constructionInputs, localManager);
  } else if(parameterDefinitionStatement_->allParameters()->size() > 0) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  classVariableManager->addLocalVariableManager(localManager);
  codeBlock_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
  classVariableManager->removeLocalVariableManager();
  
  //TODO: find a way to delete managers without destroying the values of the variables that may have been accessed
  //delete localManager;
}

void ConstructionDefinition::runAsSuperCode(ParameterSettingStatement* constructionInputs, VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) {
  VariableManager* localManager = new VariableManager();
  if(constructionInputs != nullptr) {
    convertParametersToVariablesSuperMode(constructionInputs, localManager, classVariableManager, classMethodManager);
  } else if(parameterDefinitionStatement_->allParameters()->size() > 0) {
    std::cout << "\033[1;41mERROR:\033[0m Method takes " << parameterDefinitionStatement_->allParameters()->size() << " parameters: " << sourceFile_->getFilePosition(sourceFileIndex_) << std::endl; 
    throw comp_except::MethodCallException();
  }
  classVariableManager->addLocalVariableManager(localManager);
  codeBlock_->runCodeClassMode(classVariableManager, classMethodManager, parentClass);
  classVariableManager->removeLocalVariableManager();
  
}

