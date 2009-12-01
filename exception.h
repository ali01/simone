/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a 
   Modeling & Simulation's Perspective ~ Chapter 7: Exceptions & Exception Handling */

#pragma once
#include <string>
#include <iostream>
#include <sstream>
   using std::stringstream;
using std::string;
using std::cerr;
using std::endl;

namespace Simone {

class Exception {
public:
   string what() const { return what_; }
   virtual ~Exception() {}
protected:
   Exception(char const * str) : what_(str) {
      cerr << "exception: " << str << endl;
   }
   Exception(string str) : what_(str) {
      cerr << "exception: " << str << endl;
   }
private:
   string what_;
};

class IncompleteInitializationException : public Exception {
public:
   IncompleteInitializationException(string what="") : Exception(what) {}
};

class NoImplementationException : public Exception {
public:
   NoImplementationException(string what="") : Exception(what) {} 
};

class AttributeNotSupportedException : public NoImplementationException {
public:
   AttributeNotSupportedException(string what="") : NoImplementationException(what) {} 
};

class UnknownTypeException : public Exception {
public:
   UnknownTypeException( string what="" )  : Exception(what) {}
};

class UnknownAttrException : public Exception {
public:
   UnknownAttrException( string what="" )  : Exception(what) {}
};

class UnknownDelimiterException : public Exception {
public:
   UnknownDelimiterException( string what="" )  : Exception(what) {}
};

class UnknownArgException : public Exception {
public:
   UnknownArgException( string what="" )  : Exception(what) {}
};


class RangeException : public Exception {
public:
   RangeException( string what="" ) : Exception(what) {} 
};

class MemoryException : public Exception {
 public:
   MemoryException( string what="" ) : Exception(what) {} 
};

class StorageException : public Exception {
 public:
   StorageException( string what="" ) : Exception(what) {}
};

class NameInUseException : public Exception {
public:
   NameInUseException( string what="" ) : Exception( what ) {} 
   
};

class IllegalNameException: public Exception {
 public:
   IllegalNameException( string what="" ): Exception( what ) {}
   
};

class EntityNotFoundException : public Exception {
public:
   EntityNotFoundException( string what="" ) : Exception( what ) {} 
   
};


class MemoryLimitExceededException : public MemoryException {
public:
   MemoryLimitExceededException(string what="") : MemoryException(what) {}
   
};

class ReadOnlyException : public Exception {
public:
   ReadOnlyException(string what="") : Exception(what) {}
   
};

class InvalidFormattingException: public Exception {
public:
   InvalidFormattingException(string what="") : Exception(what) {}
   
};

class InvalidOperationException: public Exception {
public:
   InvalidOperationException(string what="") : Exception(what) {}
   
};

class InvalidValueException: public Exception {
public:
   InvalidValueException(string what="") : Exception(what) {}
};

class NullPointerException : public InvalidValueException {
public:
   NullPointerException(string what="") : InvalidValueException(what) {}
};

} /* end of namespace Simone */
