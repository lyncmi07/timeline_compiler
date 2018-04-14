#ifndef MAJOR_CLASS_RUNNABLE_H
#define MAJOR_CLASS_RUNNABLE_H

class VariableManager;
class MethodManager;
class CreatedClass;

//(interface)
class ClassRunnable {
public:

  virtual ~ClassRunnable(){}

  //?? what is it using parentClass for ?? is this deprecated ??
  virtual void runCodeClassMode(VariableManager* classVariableManager, MethodManager* classMethodManager, CreatedClass* parentClass) = 0;
};

#endif
