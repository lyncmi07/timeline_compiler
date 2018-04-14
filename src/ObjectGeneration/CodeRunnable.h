#ifndef MAJOR_CODE_RUNNABLE_H
#define MAJOR_CODE_RUNNABLE_H

//(interface)
/* 
 * This class is an interface for TokenObjects that is to show that the object takes
 * some action during the running of the program.
 * This action is held in the runCode method.
 */
class CodeRunnable {
public:
  //make this class abstract
  virtual ~CodeRunnable(){}
  
  virtual void runCode() = 0;
};

#endif
