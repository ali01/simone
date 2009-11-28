#pragma once

#include "../utility.h"

namespace Simone {
namespace thread {

template <typename ChildLockable, typename BoostLockableType>
class lockable {
public:
   // virtual void lockStatusIs(int _s) = 0;
   
   BoostLockableType& boost_lockable() { return mutex_; }
protected:
   lockable() {}
   virtual ~lockable() {}
   
   BoostLockableType mutex_;
};

} /* end of namespace thread */
} /* end of namespace Simone */
