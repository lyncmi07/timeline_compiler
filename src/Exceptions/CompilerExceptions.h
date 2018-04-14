#ifndef MAJOR_NAMESPACE_COMP_EXCEPT_H
#define MAJOR_NAMESPACE_COMP_EXCEPT_H

#include <iostream>
#include <exception>

namespace comp_except {
  /**
   * This occurs when one of the files being included is invalid, whether this be because the file doesn't exist or is empty.
   */
  struct PreProcessException : public std::exception {
    const char * what () const throw () {
      return "PreProcessException";
    }
  };
  
  /**
   * This occurs when there is a syntax error in the code provided by the user.
   */
  struct TokenizerException : public std::exception {
    const char * what () const throw() {
      return "TokenizerException";
    }
  };
  
  /**
   * This occurs when the structure of the code does not fit the requirements for a valid program.
   */
  struct StructureException : public std::exception {
    const char * what() const throw() {
      return "StructureException";
    }
  };
  
  /**
   * If a method is called incorrectly then this exception will be thrown.
   */
  struct MethodCallException : public std::exception {
    const char * what() const throw() {
      return "MethodCallException";
    }
  };
  
  /**
   * If a division by 0 occurs in the user's source code this exception will be thrown
   */
  struct DivideByZeroException : public std::exception {
    const char * what() const throw() {
      return "DivideByZeroException";
    }
  };
  
  /**
   * This occurs when a variable cannot be found
   */
  struct VariableNotFoundException : public std::exception {
    const char * what() const throw() {
      return "VariableNotFoundException";
    }
  };
  
  /**
   * This occurs when an object is instantiated incorrectly
   */
  struct ObjectInstantiationException : public std::exception {
    const char * what() const throw() {
      return "ObjectInstantiationException";
    }
  };
  
  /**
   * This occurs when a value's type does not match the context.
   */
  struct TypeMismatchException : public std::exception {
    const char * what() const throw() {
      return "TypeMismatchException";
    }
  };
}

#endif
