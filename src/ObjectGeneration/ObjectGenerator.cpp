#include "ObjectGenerator.h"

#include "../Parser/Token.h"
#include "../Parser/TerminalToken.h"

#include "TokenObjects/ArrayImmediateDefinition.h"
#include "TokenObjects/ClassCodeBlock.h"
#include "TokenObjects/ClassDefinition.h"
#include "TokenObjects/CodeBlock.h"
#include "TokenObjects/ConstructionDefinition.h"
#include "TokenObjects/Equation.h"
#include "TokenObjects/Expression.h"
#include "TokenObjects/FloatImmediate.h"
#include "TokenObjects/FunctionCall.h"
#include "TokenObjects/Line.h"
#include "TokenObjects/MethodDefinition.h"
#include "TokenObjects/ObjectDefinition.h"
#include "TokenObjects/ParameterDefinition.h"
#include "TokenObjects/ParameterDefinitionStatement.h"
#include "TokenObjects/ParameterSetting.h"
#include "TokenObjects/ParameterSettingStatement.h"
#include "TokenObjects/PossibleObject.h"
#include "TokenObjects/Program.h"
#include "TokenObjects/Statement.h"
#include "TokenObjects/SubExpression.h"
#include "TokenObjects/SubVariableIdentifier.h"
#include "TokenObjects/VariableDefinition.h"
#include "TokenObjects/VariableInitiation.h"

#include "../Variables/EventManager.h"
#include "../Variables/EntityManager.h"

ObjectGenerator::ObjectGenerator(Token* programToken, VariableManager* variableManager, ClassDefinitionManager* classDefinitionManager, ValueManager* valueManager, PreProcessedFile* sourceFile) {
  generatedProgram_ = nullptr;
  eventManager_ = new EventManager();
  entityManager_ = new EntityManager();
  variableManager_ = variableManager;
  classDefinitionManager_ = classDefinitionManager;
  valueManager_ = valueManager;
  sourceFile_ = sourceFile;
  
  generatedProgram_ = (Program*)createTokenObject(programToken);
}

ObjectGenerator::~ObjectGenerator() {
  delete generatedProgram_;
  delete eventManager_;
  delete entityManager_;
}

TokenObject* ObjectGenerator::createTokenObject(Token* token) {
  switch(token->tokenRule()) {
    case PROGRAM: {
      return new Program(token->startIndex(), (CodeBlock*)createTokenObject((*token)[0])); }
    case PROGRAM2: 
      return new Program(token->startIndex(), (CodeBlock*)createTokenObject((*token)[0]), (Program*)createTokenObject((*token)[1]));
    case PROGRAM3:
      return new Program(token->startIndex(), (ClassDefinition*)createTokenObject((*token)[0]));
    case PROGRAM4:
      return new Program(token->startIndex(), (ClassDefinition*)createTokenObject((*token)[0]), (Program*)createTokenObject((*token)[1]));
    case CODE_BLOCK:
      return new CodeBlock(token->startIndex(), (Line*)createTokenObject((*token)[0]));
    case CODE_BLOCK2:
      return new CodeBlock(token->startIndex(), (Line*)createTokenObject((*token)[0]), (CodeBlock*)createTokenObject((*token)[1]));
    case LINE:
      return new Line(token->startIndex(), (Statement*)createTokenObject((*token)[0]));
    case STATEMENT:
      return new Statement(token->startIndex(), (VariableDefinition*)createTokenObject((*token)[0]));
    case STATEMENT2:
      return new Statement(token->startIndex(), (VariableInitiation*)createTokenObject((*token)[0]));
    case STATEMENT3:
      return new Statement(token->startIndex(), (FunctionCall*)createTokenObject((*token)[0]));
    case STATEMENT4:
      return new Statement(token->startIndex(), (SubVariableIdentifier*)createTokenObject((*token)[0]));
    case VARIABLE_INITIATION:
      return new VariableInitiation(token->startIndex(), variableManager_, sourceFile_, ((TerminalToken*)((*token)[0]))->getIdentifier(), ((TerminalToken*)((*token)[1]))->getIdentifier());
    case VARIABLE_DEFINITION:
      return new VariableDefinition(token->startIndex(), variableManager_, sourceFile_, (VariableInitiation*)createTokenObject((*token)[0]), (Expression*)createTokenObject((*token)[2]));
    case VARIABLE_DEFINITION2:
      return new VariableDefinition(token->startIndex(), variableManager_, sourceFile_, ((TerminalToken*)((*token)[0]))->getIdentifier(), (Expression*)createTokenObject((*token)[2]));
    case VARIABLE_DEFINITION3:
      return new VariableDefinition(token->startIndex(), variableManager_, sourceFile_, (SubVariableIdentifier*)createTokenObject((*token)[0]), (Expression*)createTokenObject((*token)[2]));
    case EXPRESSION:
      return new Expression(token->startIndex(), (SubExpression*)createTokenObject((*token)[0]));
    case EXPRESSION2:
      return new Expression(token->startIndex(), (ObjectDefinition*)createTokenObject((*token)[0]));
    case EXPRESSION3:
      return new Expression(token->startIndex(), (Equation*)createTokenObject((*token)[0]));
    case SUB_EXPRESSION:
      return new SubExpression(token->startIndex(), variableManager_, sourceFile_, valueManager_, ((TerminalToken*)((*token)[0]))->getIdentifier());
    case SUB_EXPRESSION2:
      return new SubExpression(token->startIndex(), valueManager_, ((TerminalToken*)((*token)[0]))->getInteger());
    case SUB_EXPRESSION3:
      return new SubExpression(token->startIndex(), valueManager_, (FloatImmediate*)createTokenObject((*token)[0]));
    case SUB_EXPRESSION4:
      return new SubExpression(token->startIndex(), valueManager_, ((TerminalToken*)((*token)[0]))->getStringLiteral(), true);
    case SUB_EXPRESSION5:
      return new SubExpression(token->startIndex(), valueManager_, (SubVariableIdentifier*)createTokenObject((*token)[0]));
    case SUB_EXPRESSION6:
      return new SubExpression(token->startIndex(), valueManager_, (FunctionCall*)createTokenObject((*token)[0]));
    case EQUATION: 
      return new Equation(token->startIndex(), sourceFile_, valueManager_, (SubExpression*)createTokenObject((*token)[0]), ((TerminalToken*)((*token)[1]))->getValue(), (SubExpression*)createTokenObject((*token)[2])); 
    case EQUATION2:
      return new Equation(token->startIndex(), sourceFile_, valueManager_, (SubExpression*)createTokenObject((*token)[0]), ((TerminalToken*)((*token)[1]))->getValue(), (Equation*)createTokenObject((*token)[2]));
    case OBJECT_DEFINITION:
      return new ObjectDefinition(token->startIndex(), sourceFile_, classDefinitionManager_, valueManager_, eventManager_, entityManager_, (FunctionCall*)createTokenObject((*token)[1]), (ArrayImmediateDefinition*)createTokenObject((*token)[2]));
    case FUNCTION_CALL:
      return new FunctionCall(token->startIndex(), sourceFile_, valueManager_, classDefinitionManager_, eventManager_, ((TerminalToken*)((*token)[0]))->getIdentifier(), (ParameterSettingStatement*)createTokenObject((*token)[2]));
    case FUNCTION_CALL2:
      return new FunctionCall(token->startIndex(), sourceFile_, ((TerminalToken*)((*token)[0]))->getIdentifier());
    case PARAMETER_SETTING_STATEMENT:
      return new ParameterSettingStatement(token->startIndex(), (Expression*)createTokenObject((*token)[0]));
    case PARAMETER_SETTING_STATEMENT2:
      return new ParameterSettingStatement(token->startIndex(), (Expression*)createTokenObject((*token)[0]), (ParameterSettingStatement*)createTokenObject((*token)[1]));
    case LAST_PARAMETER_SETTING:
      return (Expression*)createTokenObject((*token)[0]);
    case NOT_LAST_PARAMETER_SETTING:
      return (Expression*)createTokenObject((*token)[0]);
    case SUB_VARIABLE_IDENTIFIER:
      return new SubVariableIdentifier(token->startIndex(), sourceFile_, valueManager_, (PossibleObject*)createTokenObject((*token)[0]), (PossibleObject*)createTokenObject((*token)[2]));
    case SUB_VARIABLE_IDENTIFIER2:
      return new SubVariableIdentifier(token->startIndex(), sourceFile_, valueManager_, (PossibleObject*)createTokenObject((*token)[0]), (SubVariableIdentifier*)createTokenObject((*token)[2]));
    case POSSIBLE_OBJECT:
      return new PossibleObject(token->startIndex(), variableManager_, sourceFile_, ((TerminalToken*)((*token)[0]))->getIdentifier());
    case POSSIBLE_OBJECT2:
      return new PossibleObject(token->startIndex(), variableManager_, sourceFile_, (FunctionCall*)createTokenObject((*token)[0]));
    case ARRAY_IMMEDIATE_DEFINITION:
      return new ArrayImmediateDefinition(token->startIndex(), (ParameterSettingStatement*)createTokenObject((*token)[1]));
    case ARRAY_IMMEDIATE_DEFINITION2:
      return new ArrayImmediateDefinition(token->startIndex());
    case CLASS_DEFINITION:
      return new ClassDefinition(token->startIndex(), classDefinitionManager_, ((TerminalToken*)((*token)[1]))->getIdentifier(), ((TerminalToken*)((*token)[3]))->getIdentifier(), (ClassCodeBlock*)createTokenObject((*token)[5]));
    case CLASS_DEFINITION2:
      return new ClassDefinition(token->startIndex(), classDefinitionManager_, ((TerminalToken*)((*token)[1]))->getIdentifier(), ((TerminalToken*)((*token)[3]))->getIdentifier());
    case CLASS_CODE_BLOCK:
      return new ClassCodeBlock(token->startIndex(), sourceFile_, (CodeBlock*)createTokenObject((*token)[0]));
    case CLASS_CODE_BLOCK2:
      return new ClassCodeBlock(token->startIndex(), sourceFile_, (CodeBlock*)createTokenObject((*token)[0]), (ClassCodeBlock*)createTokenObject((*token)[1]));
    case CLASS_CODE_BLOCK3:
      return new ClassCodeBlock(token->startIndex(), sourceFile_, (MethodDefinition*)createTokenObject((*token)[0]));
    case CLASS_CODE_BLOCK4:
      return new ClassCodeBlock(token->startIndex(), sourceFile_, (MethodDefinition*)createTokenObject((*token)[0]), (ClassCodeBlock*)createTokenObject((*token)[1]));
    case CLASS_CODE_BLOCK5:
      return new ClassCodeBlock(token->startIndex(), sourceFile_, (ConstructionDefinition*)createTokenObject((*token)[0]));
    case CLASS_CODE_BLOCK6:
      return new ClassCodeBlock(token->startIndex(), sourceFile_, (ConstructionDefinition*)createTokenObject((*token)[0]), (ClassCodeBlock*)createTokenObject((*token)[1]));
    case METHOD_DEFINITION: {
      return new MethodDefinition(token->startIndex(), sourceFile_, ((TerminalToken*)((*token)[0]))->getIdentifier(), ((TerminalToken*)((*token)[1]))->getIdentifier(), (ParameterDefinitionStatement*)createTokenObject((*token)[3]), (CodeBlock*)createTokenObject((*token)[6])); }
    case METHOD_DEFINITION2:
      return new MethodDefinition(token->startIndex(), sourceFile_, ((TerminalToken*)((*token)[0]))->getIdentifier(), ((TerminalToken*)((*token)[1]))->getIdentifier(), (ParameterDefinitionStatement*)createTokenObject((*token)[3]));
    case PARAMETER_DEFINITION_STATEMENT:
      return new ParameterDefinitionStatement(token->startIndex());
    case PARAMETER_DEFINITION_STATEMENT2:
      return (ParameterDefinitionStatement*)createTokenObject((*token)[0]);
    case PARAMETER_DEFINITION_STATEMENT_FILLED:
      return new ParameterDefinitionStatement(token->startIndex(), (VariableInitiation*)createTokenObject((*token)[0]));
    case PARAMETER_DEFINITION_STATEMENT_FILLED2:
      return new ParameterDefinitionStatement(token->startIndex(), (VariableInitiation*)createTokenObject((*token)[0]), (ParameterDefinitionStatement*)createTokenObject((*token)[1]));
    case LAST_PARAMETER_DEFINITION:
      return (VariableInitiation*)createTokenObject((*token)[0]);
    case NOT_LAST_PARAMETER_DEFINITION:
      return (VariableInitiation*)createTokenObject((*token)[0]);
    case CONSTRUCTION_DEFINITION:
      return new ConstructionDefinition(token->startIndex(), sourceFile_, (ParameterDefinitionStatement*)createTokenObject((*token)[2]), (CodeBlock*)createTokenObject((*token)[5]));
    case CONSTRUCTION_DEFINITION2:
      return new ConstructionDefinition(token->startIndex(), sourceFile_, (ParameterDefinitionStatement*)createTokenObject((*token)[2]));
    case FLOAT_IMMEDIATE: {
      return new FloatImmediate(token->startIndex(), ((TerminalToken*)((*token)[0]))->getInteger(), ((TerminalToken*)((*token)[2]))->getValue()); }
    case TERMINAL:
      //TODO: this should be unreachable. throw an exception.
      return nullptr;
  }

  return nullptr;
}

Program* ObjectGenerator::generatedProgram() {
  return generatedProgram_;
}

void ObjectGenerator::manageObjects() {

}

EventManager* ObjectGenerator::eventManager() {
  return eventManager_;
}

EntityManager* ObjectGenerator::entityManager() {
  return entityManager_;
}

ValueManager* ObjectGenerator::valueManager() {
  return valueManager_;
}
