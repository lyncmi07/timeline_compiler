﻿#include <iostream>
#include <exception>

#include "Parser/PreProcessedFile.h"
#include "Parser/Tokenizer.h"
#include "ObjectGeneration/ObjectGenerator.h"
#include "ObjectGeneration/TokenObjects/Program.h"
#include "ObjectGeneration/TokenObjects/MethodDefinition.h"
#include "Variables/VariableManager.h"
#include "Variables/ClassDefinitionManager.h"
#include "Variables/ValueManager.h"
#include "Variables/EntityManager.h"
#include "Variables/EventManager.h"
#include "Variables/Entity.h"
#include "Variables/Event.h"
#include "Variables/CreatedClass.h"
#include "Variables/Variable.h"
#include "Variables/ReturnableValue.h"
#include "Exceptions/CompilerExceptions.h"

void testCode();
void simulateTimeline(EventManager* eventManager, EntityManager* entityManager);
void printToFile(std::string filename, EventManager* eventManager, EntityManager* entityManager);

int main(int argc, char* argv[]) {
  std::cout << "TPL Compiler 1.0" << std::endl;

  std::string outputFilename = "default-output.xml";

  
  std::string firstArg = argv[1];
  if(firstArg.compare("--tests") == 0) {
    //std::cout <<  tokenizer.outputString() << std::endl;
    testCode();
      
    return 0;
  }

  if(argc > 2) {
    std::string argument = argv[2];
    outputFilename = argument;
  }

  if(argc < 2) {
    std::cout << "FILE NOT GIVEN" << std::endl;
    return 0;
  }
  try {
  std::string sourceFile(argv[1]);

  PreProcessedFile ppFile(sourceFile);

  Tokenizer tokenizer(&ppFile);
  
  VariableManager variableManager;
  
  ClassDefinitionManager classDefinitionManager;
  
  ValueManager valueManager;
  
  ObjectGenerator objectGenerator(tokenizer.programToken(), &variableManager, &classDefinitionManager, &valueManager, &ppFile);

  std::cout << std::endl << "<program runtime output>" << std::endl;
  objectGenerator.generatedProgram()->runCode();
  std::cout << "</program runtime output>" << std::endl << std::endl;
  
  objectGenerator.valueManager()->allowDynamicValueUse();
  
  simulateTimeline(objectGenerator.eventManager(), objectGenerator.entityManager());

  printToFile(outputFilename, objectGenerator.eventManager(), objectGenerator.entityManager());
  
  std::cout << "\033[1;32mOutput generated successfully\033[0m" << std::endl;
  
  return 0;
  } catch(comp_except::PreProcessException& e1) {
    std::cout << "\033[1;42mINFO:\033[0m  There is a problem with the included files. Make sure all included files exist and contain code." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  } catch(comp_except::TokenizerException& e2) {
    std::cout << "\033[1;42mINFO:\033[0m  There is a syntax error in the source code." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  } catch(comp_except::StructureException& e3) {
    std::cout << "\033[1;42mINFO:\033[0m  The structure of the source code is incorrect." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  } catch(comp_except::MethodCallException& e4) {
    std::cout << "\033[1;42mINFO:\033[0m  An error has occured while attempting to call a function/method incorrectly." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  } catch(comp_except::DivideByZeroException& e5) {
    std::cout << "\033[1;42mINFO:\033[0m An equation in the source code contains a division by zero." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  } catch(comp_except::VariableNotFoundException& e6) {
    std::cout << "\033[1;42mINFO:\033[0m A variable identifier used in the source code did not refer to any variables in the program." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  } catch(comp_except::ObjectInstantiationException& e7) {
    std::cout << "\033[1;42mINFO:\033[0m An object couldn't be instantiated." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  } catch(comp_except::TypeMismatchException& e8) {
    std::cout << "\033[1;42mINFO:\033[0m A value has a type that cannot be used in the context given." << std::endl;
    std::cout << "\033[1;31mEnding program without output\033[0m" << std::endl;
  }
  return 1;
}

void simulateTimeline(EventManager* eventManager, EntityManager* entityManager) {
  std::cout << "Simulating Timeline" << std::endl << std::endl;
  eventManager->resetPointer();
  entityManager->resetPointer();
  
  bool finishedTimeline = false;
  
  //by setting these to true in the while loop the porgram will move on to processing the next event or entity
  bool eventProcessed = true;
  bool entityProcessed = true;
  
  Event* nextEvent = nullptr;
  Entity* nextEntity = nullptr;
  
  while(!finishedTimeline) {
    bool noMoreEvents = false;
    bool noMoreEntities = false;
    if(eventProcessed) {
      nextEvent = eventManager->nextEvent();
      eventProcessed = false;
    }
    if(entityProcessed) {
      nextEntity = entityManager->nextEntity();
      entityProcessed = false;
    }
    
    if(nextEntity == nullptr && nextEvent == nullptr) {
      break;
    }
    
    if(nextEvent == nullptr) {
      //no more events therefore just run through the remaining entities
      //if(nextEntity->processed()) {
        if(nextEntity->existsAtThisTime(nextEntity->startDate())) {
          nextEntity->setHappens(true);
        } else {
          nextEntity->setHappens(false);
        }
        entityProcessed = true;
    } else if(nextEntity == nullptr) {
      //no more entities therefore just run through the remaining events
      if(nextEvent->existsAtThisTime(nextEvent->eventDate())) {
        nextEvent->setHappens(true);
        //run the onEvent method to simulate the outcome of this event taking place
        nextEvent->onEventMethod()->runAsVoidMethod(nullptr, 
                                                    nextEvent->variableManager(), 
                                                    nextEvent->givenClass()->methods(),
                                                    nullptr);
      } else {
        nextEvent->setHappens(false);
      }
      eventProcessed = true;
    } else if(nextEvent->eventDate() <= nextEntity->startDate()) {
      //next event happened before next entity therefore process the event first
      if(nextEvent->existsAtThisTime(nextEvent->eventDate())) {
        nextEvent->setHappens(true);
        //run the onEvent method to simulate the outcome of this event taking place
        nextEvent->onEventMethod()->runAsVoidMethod(nullptr, 
                                                    nextEvent->variableManager(), 
                                                    nextEvent->givenClass()->methods(),
                                                    nullptr);
      } else {
        nextEvent->setHappens(false);
      }
      eventProcessed = true;
    } else {
      //next entity happened before the next event therefore process the entity first
      if(nextEntity->existsAtThisTime(nextEntity->startDate())) {
        nextEntity->setHappens(true);
      } else {
        nextEntity->setHappens(false);
      }
      entityProcessed = true;
    }
  }
}

void printToFile(std::string filename, EventManager* eventManager, EntityManager* entityManager) {
  std::cout << "Printing to file: " << filename << std::endl;
  std::ofstream outfile;
  outfile.open(filename);
  outfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  outfile << "<timeline>" << std::endl;
  //outfile << "put something here" << std::endl;
  //get output from event manager
  outfile << eventManager->printToXML(2);
  //get output from entity manager
  outfile << entityManager->printToXML(2);
  outfile << "</timeline>" << std::endl;
  outfile.close();
}

void testCode() {
  std::cout << "RUNNING TEST CODE" << std::endl;
  
  //PARSER TESTS
  std::cout << "PARSER TESTS" << std::endl;
  std::cout << "--POSITIVE TESTS (these test files should be accepted)" << std::endl;
  std::cout << "----TEST1" << std::endl;
  std::cout << "Testing following parser symbols:" << std::endl;
  std::cout << "PROGRAM2\nCODE_BLOCK2\nLINE\nSTATEMENT2\nVARIABLE_INITIATION\nSTATEMENT3\nFUNCTION_CALL\nPARAMETER_SETTING_STATEMENT\nLAST_PARAMETER_SETTING" << std::endl;
  std::cout << "EXPRESSION2\nOBJECT_DEFINITION\nFUNCTION_CALL2\nARRAY_IMMEDIATE_DEFINITION" << std::endl;
  PreProcessedFile p1("TPL_code/parser_test_code/t2.tpl");
  Tokenizer t1(&p1);
  if(t1.outputString().compare("s") == 0) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } else {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "output:" << t1.outputString() << std::endl;
  }
  
  std::cout << "----TEST2" << std::endl;
  std::cout << "Testing following parser symbols:" << std::endl;
  std::cout << "PROGRAM2\nCODE_BLOCK2\nLINE\nSTATEMENT\nVARIABLE_DEFINITION2\nEXPRESSION3\nEQUATION\nSUB_EXPRESSION2\nSUB_EXPRESSION3" << std::endl;
  std::cout << "SUB_VARIABLE_IDENTIFIER\nPOSSIBLE_OBJECT\nPOSSIBLE_OBJECT2\nFUNCTION_CALL2\nEXPRESSION\nSUB_EXPRESSION4" << std::endl;
  PreProcessedFile p2("TPL_code/parser_test_code/t3.tpl");
  Tokenizer t2(&p2);
  if(t2.outputString().compare("s") == 0) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } else {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "output:" << t2.outputString() << std::endl;
  }
  
  std::cout << "----TEST3" << std::endl;
  std::cout << "Testing following parser symbols:" << std::endl;
  std::cout << "PROGRAM2\nCODE_BLOCK2\nLINE\nSTATEMENT3\nFUNCTION_CALL\nPARAMETER_SETTING_STATEMENT2\nNOT_LAST_PARAMETER_SETTING_STATEMENT\nPARAMETER_SETTING_STATEMENT" << std::endl;
  std::cout << "LAST_PARAMETER_SETTING_STATEMENT\nSUB_EXPRESSION\nVARIABLE_DEFINITION3\nSUB_VARIABLE_IDENTIFIER2\nPOSSIBLE_OBJECT\nSUB_VARIABLE_IDENTIFIER\nEXPRESSION\n" << std::endl;
  std::cout << "VARIABLE_DEFINITION2\nSUB_EXPRESSION6\nFUNCTION_CALL2\nVARIABLE_DEFINITION\nVARIABLE_INITIATION\nEXPRESSION2\nOBJECT_DEFINITION\nARRAY_IMMEDIATE_DEFINITION" << std::endl;
  PreProcessedFile p3("TPL_code/parser_test_code/t4.tpl");
  Tokenizer t3(&p3);
  if(t3.outputString().compare("s") == 0) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } else {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "output:" << t3.outputString() << std::endl;
  }
  
  std::cout << "----TEST4" << std::endl;
  std::cout << "Testing following parser symbols:" << std::endl;
  std::cout << "PROGRAM2\nCODE_BLOCK\nPROGRAM4\nCLASS_DEFINITION2\nCLASS_DEFINITION\nCLASS_CODE_BLOCK2\nCLASS_CODE_BLOCK4\nCLASS_CODE_BLOCK5" << std::endl;
  std::cout << "METHOD_DEFINITION2\nPARAMETER_DEFINITION_STATEMENT\nCONSTRUCTION_DEFINITION2\nPARAMETER_DEFINITION_STATEMENT2" << std::endl;
  PreProcessedFile p4("TPL_code/parser_test_code/t5.tpl");
  Tokenizer t4(&p4);
  if(t4.outputString().compare("s") == 0) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } else {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "output:" << t4.outputString() << std::endl;
  }
  
  std::cout << "----TEST7" << std::endl;
  std::cout << "Testing ability to ignore whitespace during the formulation of an equation" << std::endl;
  PreProcessedFile* p7 = new PreProcessedFile("TPL_code/parser_test_code/t7.tpl");
  Tokenizer* t7 = nullptr;
  try {
    t7 = new Tokenizer(p7);
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(comp_except::TokenizerException& e3) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  }
  delete p7;
  delete t7;
  
  std::cout << "----TEST8" << std::endl;
  std::cout << "Testing the ability to handle identifiers that contain keywords in them" << std::endl;
  PreProcessedFile* p8 = new PreProcessedFile("TPL_code/parser_test_code/t8.tpl");
  Tokenizer* t8 = nullptr;
  try {
    t8 = new Tokenizer(p8);
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(comp_except::TokenizerException& e4) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  }
  delete p8;
  delete t8;
  
  std::cout << "--NEGATIVE TESTS (These test files should not be accepted)" << std::endl;
  
  std::cout << "----TEST5" << std::endl;
  std::cout << "Testing detection of a failure to place the semicolon line ending" << std::endl;
  PreProcessedFile* p5 = new PreProcessedFile("TPL_code/parser_test_code/t1.tpl");
  Tokenizer* t5 = nullptr;
  try {
    t5 = new Tokenizer(p5);
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    
  } catch(comp_except::TokenizerException& e1) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  }
  delete p5;
  delete t5;
  
  std::cout << "----TEST6" << std::endl;
  std::cout << "Testing detection of  a failure to place the dependency array during object instantiation" << std::endl;
  PreProcessedFile* p6 = new PreProcessedFile("TPL_code/parser_test_code/t6.tpl");
  Tokenizer* t6 = nullptr;
  try {
    t6 = new Tokenizer(p6);
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::TokenizerException& e2) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  }
  delete p6;
  delete t6;
  
  std::cout << "----TEST9" << std::endl;
  std::cout << "Test incorrect keyword spelling" << std::endl;
  PreProcessedFile* p9 = new PreProcessedFile("TPL_code/parser_test_code/t9.tpl");
  Tokenizer* t9 = nullptr;
  try {
    t9 = new Tokenizer(p9);
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::TokenizerException& e5) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  }
  delete p9;
  delete t9;

  //PROGRAM EXECUTION TESTS
  std::cout << "PROGRAM EXECUTION TESTS" << std::endl;
  std::cout << "--POSITIVE TESTS (these test files should be accepted)" << std::endl;
  
  PreProcessedFile* ppf = nullptr;
  Tokenizer* tkr = nullptr;
  VariableManager* vm = nullptr;
  ClassDefinitionManager* cdm = nullptr;
  ValueManager* valMan = nullptr;
  ObjectGenerator* og = nullptr;
  
  std::cout << "----TEST10" << std::endl;
  std::cout << "Test for the functionality of the 'super' function" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t10.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    Variable* franco = vm->getVariable("franco");
    if(franco == nullptr) {
      std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
      std::cout << "variable doesn't exist" << std::endl;  
    }
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    franco->value()->objectValue()->variableManager()->listGeneratedVariables();
  } catch(comp_except::TokenizerException& e6) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "Test failed during parser stage" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST11" << std::endl;
  std::cout << "Test for an object's ability to reference other objects" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t11.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    Variable* franco = vm->getVariable("franco");
    if(franco == nullptr) {
      std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
      std::cout << "variable doesn't exist" << std::endl;  
    }
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    franco->value()->objectValue()->variableManager()->listGeneratedVariables();
  } catch(comp_except::TokenizerException& e6) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "Test failed during parser stage" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST12" << std::endl;
  std::cout << "Test for an object's ability to reference objects of it's own kind using a setting method" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t12.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    Variable* franco = vm->getVariable("franco");
    if(franco == nullptr) {
      std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
      std::cout << "variable doesn't exist" << std::endl;  
    }
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    franco->value()->objectValue()->variableManager()->listGeneratedVariables();
  } catch(comp_except::TokenizerException& e6) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "Test failed during parser stage" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST13" << std::endl;
  std::cout << "Test return method" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t13.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    Variable* francoName = vm->getVariable("francoFull");
    if(francoName == nullptr) {
      std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
      std::cout << "variable doesn't exist" << std::endl;  
    }
    if(francoName->value()->stringLiteralValue().compare("General Franco of Nationalist Spain") == 0) {
      std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
    } else {
      std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
      std::cout << "Should be:    'General Franco of Nationalist Spain'" << std::endl;
      std::cout << "Actual value: '" << francoName->value()->stringLiteralValue() << "'" << std::endl;
    }
  } catch(comp_except::TokenizerException& e6) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "Test failed during parser stage" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST14" << std::endl;
  std::cout << "Test println and print methods" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t14.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    og->generatedProgram()->runCode();
  } catch(comp_except::TokenizerException& e6) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "Test failed during parser stage" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST15" << std::endl;
  std::cout << "Test string concatenation inside println function" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t15.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    og->generatedProgram()->runCode();
  } catch(comp_except::TokenizerException& e6) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << "Test failed during parser stage" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "--NEGATIVE TESTS (These test files should not be accepted)" << std::endl;
  
  std::cout << "----TEST16" << std::endl;
  std::cout << "Ensure error thrown on attempts to call non-existent methods" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t16.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::MethodCallException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST17" << std::endl;
  std::cout << "Using incorrect datatypes in method parameters" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t17.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::MethodCallException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST18" << std::endl;
  std::cout << "Using no parameters for constructor that takes them" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t18.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::MethodCallException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST19" << std::endl;
  std::cout << "Using no parameters for funcions that take them" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t19.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::MethodCallException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST20" << std::endl;
  std::cout << "Using no parameters for methods that take them" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t20.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::MethodCallException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST21" << std::endl;
  std::cout << "Use incorrect types in the object dependencies" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t21.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::TypeMismatchException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST22" << std::endl;
  std::cout << "Use a function that doesn't return anything in object dependencies" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t22.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::MethodCallException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST23" << std::endl;
  std::cout << "Use a method that doesn't return anything in object dependencies" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t23.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
  } catch(comp_except::TypeMismatchException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST24" << std::endl;
  std::cout << "Use the GetEventContainingDataValue method in an area of the code that isn't appropriate (outside a class)" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t24.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::StructureException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST25" << std::endl;
  std::cout << "Use the GetEventContainingDataValue method in an area of the code that isn't appropriate (inside a class)" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/runtime_test_code/t25.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
  } catch(comp_except::StructureException& e6) {
    std::cout << "\033[1;43m---TEST PASSED---\033[0m" << std::endl;
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  //TIMELINE SIMULATION TESTS
  std::cout << "TIMELINE SIMULATION TESTS" << std::endl;
  std::cout << "--POSITIVE TESTS (these test files should be accepted)" << std::endl;
  
  std::cout << "----TEST26" << std::endl;
  std::cout << "Program with no dependencies. Should make all events and entities happen" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/timeline_test_code/t26.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    og->valueManager()->allowDynamicValueUse();
    simulateTimeline(og->eventManager(), og->entityManager());
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    std::cout << "Look in /tests/t26.xml" << std::endl;
    printToFile("./tests/t26.xml", og->eventManager(), og->entityManager());
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST27" << std::endl;
  std::cout << "Program containing dependencies. Should make all events and entities happen" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/timeline_test_code/t27.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    og->valueManager()->allowDynamicValueUse();
    simulateTimeline(og->eventManager(), og->entityManager());
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    std::cout << "Look in /tests/t27.xml" << std::endl;
    printToFile("./tests/t27.xml",og->eventManager(), og->entityManager());
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST28" << std::endl;
  std::cout << "Program containing dependencies. The spanishWar should be avoided" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/timeline_test_code/t28.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    og->valueManager()->allowDynamicValueUse();
    simulateTimeline(og->eventManager(), og->entityManager());
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    std::cout << "Look in /tests/t28.xml" << std::endl;
    printToFile("./tests/t28.xml",og->eventManager(), og->entityManager());
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
  
  std::cout << "----TEST29" << std::endl;
  std::cout << "Use the GetEventContainingDataValue function. The function should pick up the dictator of the spanishWar in this function" << std::endl;
  try {
    ppf = new PreProcessedFile("TPL_code/timeline_test_code/t29.tpl");
    tkr = new Tokenizer(ppf);
    vm = new VariableManager();    
    cdm = new ClassDefinitionManager();
    valMan = new ValueManager();
    og = new ObjectGenerator(tkr->programToken(), vm, cdm, valMan, ppf);
    og->generatedProgram()->runCode();
    og->valueManager()->allowDynamicValueUse();
    simulateTimeline(og->eventManager(), og->entityManager());
    std::cout << "\033[1;44m---CHECK MANUALLY---\033[0m" << std::endl;
    std::cout << "Look in /tests/t29.xml" << std::endl;
    printToFile("./tests/t29.xml",og->eventManager(), og->entityManager());
  } catch(std::exception& e7) {
    std::cout << "\033[1;41m---TEST FAILED---\033[0m" << std::endl;
    std::cout << e7.what() << " was thrown" << std::endl;
  }
  delete ppf;
  delete tkr;
  delete vm;
  delete cdm;
  delete valMan;
  delete og;
}


