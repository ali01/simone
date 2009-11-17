#ifndef SET_H_31X1QINF
#define SET_H_31X1QINF

#include <set>
#include <functional>
#include "ptr.h"

namespace Simone {
using std::set;
using std::less;
using std::allocator;

template <typename T,
          typename Compare=less<T>,
          typename Allocator=allocator<T> >
class Set : public Simone::PtrInterface<Set<T,Compare,Allocator> > {
public: // todo: support boost_foreach
   // type declarations ==============================================================
   typedef Simone::Ptr<const Set<T,Compare,Allocator> > PtrConst;
   typedef Simone::Ptr<Set<T,Compare,Allocator> > Ptr;
   
   typedef typename set<T,Compare,Allocator>::iterator               iterator;
   typedef typename set<T,Compare,Allocator>::reverse_iterator       reverse_iterator;
   typedef typename set<T,Compare,Allocator>::const_iterator         const_iterator;
   typedef typename set<T,Compare,Allocator>::const_reverse_iterator
                                                               const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr SetNew() { return new Set(); }
   Set() {} // allow static allocation
   
   // iterators ======================================================================
   iterator               begin()        { return set_.begin();  }
   iterator               end()          { return set_.end();    }
   reverse_iterator       rbegin()       { return set_.rbegin(); }
   reverse_iterator       rend()         { return set_.rend();   }
   
   const_iterator         begin()  const { return set_.begin();  }
   const_iterator         end()    const { return set_.end();    }
   const_reverse_iterator rbegin() const { return set_.rbegin(); }
   const_reverse_iterator rend()   const { return set_.rend();   }
   
   // accessors  =====================================================================
   size_t size()  const { return set_.size();  }
   bool   empty() const { return set_.empty(); }
   
         iterator element(const T& _v)       { return set_.find(_v); }
   const_iterator element(const T& _v) const { return set_.find(_v); }
   
   size_t count(const T& _v) const { return set_.count(_v); }
   
   // mutators =======================================================================
   void elementIs(const T& _v) { set_.insert(_v); }
   void elementDel(const T& _v) { set_.erase(_v); }
protected:
private:
   // member functions ===============================================================
   // data members ===================================================================
   set<T,Compare,Allocator> set_;
   // disallowed operations ==========================================================
   Set(const Set&);
   void operator=(const Set&);
};

} /* end of namespace Simone */

#endif
