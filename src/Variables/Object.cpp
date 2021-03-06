#include "Object.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include "CreatedClass.h"
#include "VariableManager.h"
#include "../ObjectGeneration/TokenObjects/ArrayImmediateDefinition.h"
#include "../ObjectGeneration/TokenObjects/Expression.h"
#include "../ObjectGeneration/TokenObjects/ConstructionDefinition.h"
#include "../Exceptions/CompilerExceptions.h"
#include "ReturnableValue.h"
#include "Variable.h"

#include <iostream>

Object::Object(CreatedClass* givenClass, ParameterSettingStatement* constructorParameters, ArrayImmediateDefinition* dependencyArray) {
  happens_ =  false;
  processed_ = false;
  givenClass_ = givenClass;
  //Get variables from class
  variableManager_ = new VariableManager(*(givenClass->subVariables()));
  
  //run the constructor using the constructor parameters
  givenClass->constructionDefinition()->runAsConstructionCode(constructorParameters, variableManager_, givenClass->methods(), givenClass->parentClass());
  
  
  if(dependencyArray->allParameters() != nullptr) {
    for(int i = 0; i < dependencyArray->allParameters()->size(); i++) {
      ReturnableValue* value = dependencyArray->allParameters()->at(i)->returningCode();
      if(value != nullptr && value->isObject()) {
        dependencies_.push_back(value);
      } else {
        std::cout << "\033[1;41mERROR:\033[0m Can only be dependent on other objects" << std::endl;
        throw comp_except::TypeMismatchException();
      }
    }
  }
}

Object::~Object() {
  delete variableManager_;
}

std::string Object::toString() {
  std::string returnValue = givenClass_->className() + "[\n";
  for(int i = 0; i < variableManager_->allVariables()->size(); i++) {
    returnValue += variableManager_->allVariables()->at(i)->toString() + "\n";
  }
  returnValue += "]";
  return returnValue;
}

VariableManager* Object::variableManager() {
  return variableManager_;
}

CreatedClass* Object::givenClass() {
  return givenClass_;
}

std::vector<ReturnableValue*>* Object::dependencies() {
  return &dependencies_;
}

bool Object::happens() {
  return happens_;
}

void Object::setHappens(bool doesItHappen) {
  happens_ = doesItHappen;
}

bool Object::processed() {
  return processed_;
}

void Object::finishedProcessing() {
  processed_ = true;
}

std::string Object::printToXML(int spaceIndent) {
  std::string returnString = "";
  std::string indent(spaceIndent, ' ');
  std::string indent2(spaceIndent + 2, ' ');
  size_t objectAddress = reinterpret_cast<std::size_t>(this);
  std::stringstream id_stream;
  id_stream << std::hex << objectAddress;
  std::string id(id_stream.str());
  returnString += indent + "<object type=\"" + givenClass_->className() 
                         + "\" happened=\"" + (happens_ ? "true":"false") + "\""
                         + " id=\"" + id + "\">\n";
  returnString += indent2 + "<dependencies>\n";
  for(int i = 0; i < dependencies_.size(); i++) {
    returnString += indent2 + "  " + dependencies_[i]->printToXML(spaceIndent+2) + "\n";
  }
  returnString += indent2 + "</dependencies>\n";
  returnString += variableManager()->printToXML(spaceIndent+2);
  returnString += indent + "</object>\n";
  
  return returnString;
}








