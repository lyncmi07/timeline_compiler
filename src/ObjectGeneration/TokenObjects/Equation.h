#ifndef MAJOR_EQUATION_H
#define MAJOR_EQUATION_H

#include "TokenObject.h"
#include "../ValueReturnable.h"
#include "../ClassValueReturnable.h"

#include <string>
#include <vector>

class SubExpression;
class PreProcessedFile;
class ValueManager;

typedef enum {
  MINUS,
  PLUS,
  MULTIPLY,
  DIVIDE
} Operator;

class Equation : public TokenObject, ValueReturnable, ClassValueReturnable {
private:
//owned memory
std::vector<SubExpression*> equationPieces_;
std::vector<Operator> equationOperators_;

//memory not owned
PreProcessedFile* sourceFile_; 
ValueManager* valueManager_;
public:
  //EQUATION
  Equation(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, SubExpression* subExpression1, std::string givenOperator, SubExpression* subExpression2);
  //EQUATION2
  Equation(int sourceFileIndex, PreProcessedFile* sourceFile, ValueManager* valueManager, SubExpression* subExpression, std::string givenOperator, Equation* equation); 

  //copy constructor
  Equation(const Equation &original);

  ~Equation();

  std::vector<SubExpression*>* equationPieces();
  std::vector<Operator>* equationOperators();

  ReturnableValue* returningCode() override;  
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
