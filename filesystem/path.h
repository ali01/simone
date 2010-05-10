#ifndef PATH_H_A8519LVB
#define PATH_H_A8519LVB

/* stl includes */
#include <string>

/* boost includes */
#include <boost/filesytem/path.hpp>

/* simone includes */
#include "../ptr_interface.h"

namespace Simone {

class Path : public PtrInterface<Path> {
public:
  typedef Simone::Ptr<const Path> PtrConst;
  typedef Simone::Ptr<Path> Ptr;
  
  static Ptr PathNew(const string& path) {
    return new Path(path);
  }
  
  const std::string& str() const { return path_.string(); }

private:
  Path(const string& path) : path_(path, boost::filesystem::native) {}
  
  /* data members */
  boost::filesystem::path path_;
};

} /* end of namespace Simone */

#endif
