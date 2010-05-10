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
  
  static PtrConst PathNew(const string& _path_str) {
    return new Path(_path_str);
  }
  
  const std::string& str() const { return path_.string(); }

private:
  Path(const string& _path_str) : path_(_path_str, boost::filesystem::native) {}
  
  /* data members */
  boost::filesystem::path path_;
};

} /* end of namespace Simone */

#endif
