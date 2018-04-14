# Software System

### Installation:
This software requires a linux machine with 
- Linux Machine (Preferably Ubuntu)
- GNU Make (Preferably version 4)
- g++ (Perferably >=5.2.1)
- 

There is an issue with svn that keeps deleting one of my source files (/src/ObjectGeneration/ClassValueReturnable.h).
Should this happens, there is another file in the same location (ClassValueReturnable.h.backup) that holds 
the same source code. Please put the contents of this file into a new ClassValueReturnable.h file if it goes missing.

From this folder (with all the contents of the folder) run the following commands from the terminal:
```sh
make clean
make
```
This will generate the compiler in a file called "main".
This file can be copied anywhere although running tests requires the TPL_code folder to be in the same folder as the executable.

### Running the Executable:
This program can be run in multiple ways.

```sh
./main input_file.tpl
```
The first argument ("input_file.tpl") is the file path to the file that is intented to be compiled. This fill be compiled into the "default-output.xml" file in the directory where the compiler "main" exists.

```sh
./main input_file.tpl output_file.xml
```
This method of execution allows for the user to set what output file should be written to ("output_file.xml") rather than using the default output ("default-output.xml").

```sh
./main --tests
```
This is used for running the included tests in the TPL_code folder. It is not intended for mainstream use of the program. If you would like to check what tests are being run they can be viewed in "./src/main.cpp"

### Included Files:
Makefile
main
tests/
obj/
src/main.cpp
src/Exceptions/ComplilerExceptions.h
src/ObjectGeneration/ClassRunnable.h
src/ObjectGeneration/CodeRunnable.h
src/ObjectGeneration/ClassValueReturnable.h
src/ObjectGeneration/ValueReturnable.h
src/ObjectGeneration/ObjectGenerator.cpp
src/ObjectGeneration/ObjectGenerator.h
src/ObjectGeneration/TokenObjects/ArrayImmediateDefinition.cpp
src/ObjectGeneration/TokenObjects/ArrayImmediateDefinition.h
src/ObjectGeneration/TokenObjects/ClassCodeBlock.cpp
src/ObjectGeneration/TokenObjects/ClassCodeBlock.h
src/ObjectGeneration/TokenObjects/ClassDefinition.cpp
src/ObjectGeneration/TokenObjects/ClassDefinition.h
src/ObjectGeneration/TokenObjects/CodeBlock.cpp
src/ObjectGeneration/TokenObjects/CodeBlock.h
src/ObjectGeneration/TokenObjects/ConstructionDefinition.cpp
src/ObjectGeneration/TokenObjects/ConstructionDefinition.h
src/ObjectGeneration/TokenObjects/Equation.cpp
src/ObjectGeneration/TokenObjects/Equation.h
src/ObjectGeneration/TokenObjects/Expression.cpp
src/ObjectGeneration/TokenObjects/Expression.h
src/ObjectGeneration/TokenObjects/FloatImmediate.cpp
src/ObjectGeneration/TokenObjects/FloatImmediate.h
src/ObjectGeneration/TokenObjects/FunctionCall.cpp
src/ObjectGeneration/TokenObjects/FunctionCall.h
src/ObjectGeneration/TokenObjects/Line.cpp
src/ObjectGeneration/TokenObjects/Line.h
src/ObjectGeneration/TokenObjects/MethodDefinition.cpp
src/ObjectGeneration/TokenObjects/MethodDefinition.h
src/ObjectGeneration/TokenObjects/ObjectDefinition.cpp
src/ObjectGeneration/TokenObjects/ObjectDefinition.h
src/ObjectGeneration/TokenObjects/ParameterDefinition.cpp
src/ObjectGeneration/TokenObjects/ParameterDefinition.h
src/ObjectGeneration/TokenObjects/ParameterDefinitionStatement.cpp
src/ObjectGeneration/TokenObjects/ParameterDefinitionStatement.h
src/ObjectGeneration/TokenObjects/ParameterSetting.cpp
src/ObjectGeneration/TokenObjects/ParameterSetting.h
src/ObjectGeneration/TokenObjects/ParameterSettingStatement.cpp
src/ObjectGeneration/TokenObjects/ParameterSettingStatement.h
src/ObjectGeneration/TokenObjects/PossibleObject.cpp
src/ObjectGeneration/TokenObjects/PossibleObject.h
src/ObjectGeneration/TokenObjects/Program.cpp
src/ObjectGeneration/TokenObjects/Program.h
src/ObjectGeneration/TokenObjects/Statement.cpp
src/ObjectGeneration/TokenObjects/Statement.h
src/ObjectGeneration/TokenObjects/SubExpression.cpp
src/ObjectGeneration/TokenObjects/SubExpression.h
src/ObjectGeneration/TokenObjects/SubVariableIdentifier.cpp
src/ObjectGeneration/TokenObjects/SubVariableIdentifier.h
src/ObjectGeneration/TokenObjects/TokenObject.cpp
src/ObjectGeneration/TokenObjects/TokenObject.h
src/ObjectGeneration/TokenObjects/VariableDefinition.cpp
src/ObjectGeneration/TokenObjects/VariableDefinition.h
src/ObjectGeneration/TokenObjects/VariableInitiation.cpp
src/ObjectGeneration/TokenObjects/VariableInitiation.h
src/Helpers/StringFunctions.cpp
src/Helpers/StringFunctions.h
src/Parser/PreProcessedFile.cpp
src/Parser/PreProcessedFile.h
src/Parser/TerminalToken.cpp
src/Parser/TerminalToken.h
src/Parser/Token.cpp
src/Parser/Token.h
src/Parser/Tokenizer.cpp
src/Parser/Tokenizer.h
src/Variables/ClassDefinitionManager.cpp
src/Variables/ClassDefinitionManager.h
src/Variables/CreatedClass.cpp
src/Variables/CreatedClass.h
src/Variables/DynamicReturnableValue.cpp
src/Variables/DynamicReturnableValue.h
src/Variables/Entity.cpp
src/Variables/Entity.h
src/Variables/EntityManager.cpp
src/Variables/EntityManager.h
src/Variables/Event.cpp
src/Variables/Event.h
src/Variables/EventManager.cpp
src/Variables/EventManager.h
src/Variables/MethodManager.cpp
src/Variables/MethodManager.h
src/Variables/Object.cpp
src/Variables/Object.h
src/Variables/ReturnableValue.cpp
src/Variables/ReturnableValue.h
src/Variables/ValueManager.cpp
src/Variables/ValueManager.h
src/Variables/Variable.cpp
src/Variables/Variable.h
src/Variables/VariableManager.cpp
src/Variables/VariableManager.h
TPL_code/example.tpl
TPL_code/hello.tpl
TPL_code/main.tpl
TPL_code/rebecca.tpl
TPL_code/world.tpl
TPL_code/parser_test_code/t1.tpl
TPL_code/parser_test_code/t2.tpl
TPL_code/parser_test_code/t3.tpl
TPL_code/parser_test_code/t4.tpl
TPL_code/parser_test_code/t5.tpl
TPL_code/parser_test_code/t6.tpl
TPL_code/parser_test_code/t7.tpl
TPL_code/parser_test_code/t8.tpl
TPL_code/parser_test_code/t9.tpl
TPL_code/runtime_test_code/t10.tpl
TPL_code/runtime_test_code/t11.tpl
TPL_code/runtime_test_code/t12.tpl
TPL_code/runtime_test_code/t13.tpl
TPL_code/runtime_test_code/t14.tpl
TPL_code/runtime_test_code/t15.tpl
TPL_code/runtime_test_code/t16.tpl
TPL_code/runtime_test_code/t17.tpl
TPL_code/runtime_test_code/t18.tpl
TPL_code/runtime_test_code/t19.tpl
TPL_code/runtime_test_code/t20.tpl
TPL_code/runtime_test_code/t21.tpl
TPL_code/runtime_test_code/t22.tpl
TPL_code/runtime_test_code/t23.tpl
TPL_code/runtime_test_code/t24.tpl
TPL_code/runtime_test_code/t25.tpl
TPL_code/timeline_test_code/t26.tpl
TPL_code/timeline_test_code/t27.tpl
TPL_code/timeline_test_code/t28.tpl
TPL_code/timeline_test_code/t29.tpl
