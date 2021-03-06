#ifndef MAJOR_SUB_EXPRESSION_H
#define MAJOR_SUB_EXPRESSION_h

#include "TokenObject.h"
#include <string>
#include "../ValueReturnable.h"
#include "../ClassValueReturnable.h"

class FloatImmediate;
class SubVariableIdentifier;
class FunctionCall;
class ReturnableValue;
class VariableManager;
class PreProcessedFile;
class ValueManager;

typedef enum {
  SE_ID,
  SE_INT,
  SE_FLOAT,
  SE_SL,
  SE_SVI,
  SE_FC
} SubExpressionType;

class SubExpression : public TokenObject, ValueReturnable, ClassValueReturnable {
private:
  SubExpressionType type_;
  std::string identifier_;
  int integerValue_;
  float floatValue_;
  std::string stringLiteral_;
  
  //owned memory
  SubVariableIdentifier* subVariableIdentifier_;
  FunctionCall* functionCall_;
  
  //memory not owned
  VariableManager* variableManager_;
  PreProcessedFile* sourceFile_;
  ValueManager* valueManager_;
public:
  //SUB_EXPRESSION
  SubExpression(int sourceFileIndex, VariableManager* variableManager, PreProcessedFile* sourceFile, ValueManager* valueManager, std::string identifier);
  //SUB_EXPRESSION2
  SubExpression(int sourceFileIndex, ValueManager* valueManager, int integerValue);
  //SUB_EXPRESSION3
  SubExpression(int sourceFileIndex, ValueManager* valueManager, FloatImmediate* floatValue);
  //SUB_EXPRESSION4
  SubExpression(int sourceFileIndex, ValueManager* valueManager, std::string stringLiteral, bool usingString);
  //SUB_EXPRESSION5
  SubExpression(int sourceFileIndex, ValueManager* valueManager, SubVariableIdentifier* subVariableIdentifier);
  //SUB_EXPRESSION6
  SubExpression(int sourceFileIndex, ValueManager* valueManager, FunctionCall* functionCall);
  
  //copy constructor
  SubExpression(const SubExpression &original);
  
  ~SubExpression();
  
  bool immediateRun();
  
  ReturnableValue* returningCode() override;
  ReturnableValue* returningCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) override;
};

#endif
