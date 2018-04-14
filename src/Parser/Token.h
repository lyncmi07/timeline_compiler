#ifndef MAJOR_TOKEN_H
#define MAJOR_TOKEN_H

#include <iostream>
#include <vector>

typedef enum {
  identifierSym,
  stringLiteralSym,
  integerSym,
  floatSym,
  operatorSym,
  commentStartSym,
  commentEndSym,
  semicolonSym,
  equalsSym,
  newSym,			//'new'
  openParenthasisSym,
  closeParenthasisSym,
  commaSym,
  openSquareBracketSym,
  closeSquareBracketSym,
  voidSym,			//'void'
  periodSym,
  openCurlyBracesSym,
  closeCurlyBracesSym,
  constructSym,			//'_construct'
  classSym,			//'class'
  extendsSym,			//'extends'
  nt_programSym,
  nt_codeBlockSym,
  nt_lineSym,
  nt_statementSym,
  nt_variableInitiationSym,
  nt_variableDefinitionSym,
  nt_expressionSym,
  nt_subExpressionSym,
  nt_equationSym,
  nt_objectDefinitionSym,
  nt_functionCallSym,
  nt_parameterSettingStatementSym,
  nt_lastParameterSettingSym,
  nt_notLastParameterSettingSym,
  nt_subVariableIdentifierSym,
  nt_possibleObjectSym,
  nt_arrayImmediateDefinitionSym,
  nt_classDefinitionSym,
  nt_classCodeBlockSym,
  nt_methodDefinitionSym,
  nt_parameterDefinitionStatementSym,
  nt_parameterDefinitionStatementFilledSym,
  nt_lastParameterDefinitionSym,
  nt_notLastParameterDefinitionSym,
  nt_constructionDefinitionSym,
  nt_floatImmediateSym,             
  noSymbol,
  errorSym,
  eofSymbol,
  iterateSymbol		//this is used by the nextSymbol() function in Tokenizer to note that the function needs to run again to find the correct symbol
} Symbol;

typedef enum {//those marked with t have been written into a test
  PROGRAM,			//<code block> t
  PROGRAM2,			//<code block> <program> t
  PROGRAM3,			//<class definition> t
  PROGRAM4,			//<class definition> <program> t
  CODE_BLOCK,			//<line> t
  CODE_BLOCK2,			//<line> <code block> t
  LINE,				//<statement> ';' t
  STATEMENT,			//<variable definition> t
  STATEMENT2,			//<variable initiation> t
  STATEMENT3,			//<function call> t
  STATEMENT4,			//<sub variable identifier>
  VARIABLE_INITIATION,		//identifier identifier t
  VARIABLE_DEFINITION,		//<variable initiation> '=' <expression> t
  VARIABLE_DEFINITION2,		//identifier '=' <expression> t
  VARIABLE_DEFINITION3,		//<sub variable identifier> '=' <expression> t
  EXPRESSION,			//<subexpression>t
  EXPRESSION2,			//<object definition> t
  EXPRESSION3,			//<equation> t
  SUB_EXPRESSION,		//identifier t
  SUB_EXPRESSION2,		//integer t
  SUB_EXPRESSION3,		//<float> t
  SUB_EXPRESSION4,		//string literal t
  SUB_EXPRESSION5,		//<sub variable identifier> t
  SUB_EXPRESSION6,		//<function call> t
  EQUATION,			//<sub expression> Operator <sub expression> t
  EQUATION2,			//<sub expression> Operator <equation> t
  OBJECT_DEFINITION,		//'new' <function call> <array immedaite definition> t
  FUNCTION_CALL,		//identifier '(' <parameter setting statement> ')' t
  FUNCTION_CALL2,		//identifier '(' ')' t
  PARAMETER_SETTING_STATEMENT,	//<last parameter setting> t
  PARAMETER_SETTING_STATEMENT2,	//<not last parameter setting> <parameter setting statement> t
  LAST_PARAMETER_SETTING,	//<expression> t
  NOT_LAST_PARAMETER_SETTING,	//<expression> ',' t
  SUB_VARIABLE_IDENTIFIER,	//<possible object> '.' <possible object> t
  SUB_VARIABLE_IDENTIFIER2,	//<possible object> '.' <sub variable identifier> t
  POSSIBLE_OBJECT,		//identifier t
  POSSIBLE_OBJECT2,		//<function call> t
  ARRAY_IMMEDIATE_DEFINITION,	//'[' <parameter setting statement> ']' t
  ARRAY_IMMEDIATE_DEFINITION2,   //'[' ']' t
  CLASS_DEFINITION,		//'class' identifier 'extends' identifier '{' <class code block> '}' t
  CLASS_DEFINITION2,		//'class' identifier 'extends' identifier '{' '}' t
  CLASS_CODE_BLOCK,		//<code block> t
  CLASS_CODE_BLOCK2,		//<code block> <class code block> t
  CLASS_CODE_BLOCK3,		//<method definition> t
  CLASS_CODE_BLOCK4,		//<method definition> <class code block> t
  CLASS_CODE_BLOCK5,		//<construction definition> t
  CLASS_CODE_BLOCK6,		//<construction definition> <class code block> t
  METHOD_DEFINITION,		//identifier identifier '(' <parameter definition statement> ')' '{' <code block> '}' t
  METHOD_DEFINITION2,		//identifier identifier '(' <parameter definition statement> ')' '{' '}' t
  PARAMETER_DEFINITION_STATEMENT,		//'void' t
  PARAMETER_DEFINITION_STATEMENT2,		//<parameter definition statement filled> t
  PARAMETER_DEFINITION_STATEMENT_FILLED,	//<last parameter definition> t
  PARAMETER_DEFINITION_STATEMENT_FILLED2,	//<not last parameter definition> <parameter definition statement filled> t
  LAST_PARAMETER_DEFINITION,			//<variable initiation> t
  NOT_LAST_PARAMETER_DEFINITION,		//<variable initiation> ',' t
  CONSTRUCTION_DEFINITION,	//'_construct' '(' <parameter definition statement> ')' '{' <code block> '}' t
  CONSTRUCTION_DEFINITION2,	//'_construct' '(' <parameter definition statement> ')' '{' '}' t
  FLOAT_IMMEDIATE,		//integerSym '.' integerSym t
  TERMINAL			//Token does not hold a rule
} SyntaxRule;

class Token {
private:
  Symbol tokenSymbol_;
  SyntaxRule tokenRule_;

  //owns this memory
  std::vector<Token*> subTokens_;
  int currentTokenIndex_;
  int startIndex_;

public:

  //Destructor: deletes all sub tokens.
  ~Token();

  //generates a new Token object.
  //tokenSymbol: the type of symbol this token is
  //tokenRule: The version of this symbol
  //startIndex: the index in the source code that this symbol occurs
  Token(Symbol tokenSymbol, SyntaxRule tokenRule, int startIndex);



  //returns true if the object has sub tokens
  bool isNonTerminal();

  //adds a token to the subTokens_ list
  void addNextToken(Token* nextToken);

  Symbol tokenSymbol() {
    return tokenSymbol_;
  }
  SyntaxRule tokenRule() {
    return tokenRule_;
  }

  std::vector<Token*> subTokens() {
    return subTokens_;
  }
  
  Token* operator[](int i);

  int startIndex() {
    return startIndex_;
  }
};

#endif
