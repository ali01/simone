/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a 
   Modeling & Simulation's Perspective ~ Chapter 7: Exceptions & Exception Handling */

#pragma once
#include <string>
using std::string;

namespace Simone {

class Exception {
public:
   string what() const { return what_; }
   virtual ~Exception() {}
protected:
   Exception(char const * str) : what_(str) {}
   Exception(string str) : what_(str) {}
private:
   string what_;
};

class IncompleteInitializationException : public Exception {
public:
   IncompleteInitializationException(string what) : Exception(what) {}
};

class NoImplementationException : public Exception {
public:
   NoImplementationException(string what) : Exception(what) {} 
};

class AttributeNotSupportedException : public NoImplementationException {
public:
   AttributeNotSupportedException(string what) : NoImplementationException(what) {} 
};

} /* end of namespace Simone */
