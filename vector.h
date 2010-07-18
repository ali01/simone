/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#ifndef VECTOR_H_563FAR15
#define VECTOR_H_563FAR15

#include <vector>
using std::vector;

#include "ptr_interface.h"
#include "utility.h"

namespace Simone {

template <typename T>
class Vector : public PtrInterface<Vector<T> > {
public:
   // type declarations ========================================================
   typedef Simone::Ptr<const Vector<T> > PtrConst;
   typedef Simone::Ptr<Vector<T> > Ptr;

   typedef typename vector<T>::iterator               iterator;
   typedef typename vector<T>::reverse_iterator       reverse_iterator;
   typedef typename vector<T>::const_iterator         const_iterator;
   typedef typename vector<T>::const_reverse_iterator const_reverse_iterator;

   // factory constructor ======================================================
   static Ptr VectorNew() { return new Vector(); }
   Vector() {}
   virtual ~Vector() {}

   // iterators ================================================================
   iterator               begin()  { return vector_.begin();  }
   iterator               end()    { return vector_.end();    }
   reverse_iterator       rbegin() { return vector_.rbegin(); }
   reverse_iterator       rend()   { return vector_.rend();   }

   const_iterator         begin()  const { return vector_.begin();  }
   const_iterator         end()    const { return vector_.end();    }
   const_reverse_iterator rbegin() const { return vector_.rbegin(); }
   const_reverse_iterator rend()   const { return vector_.rend();   }

   // accessors  ===============================================================
   size_t size() const { return vector_.size(); }
   bool   empty() const { return vector_.empty(); }
   T& element(uint32_t _i) { return vector_[_i]; }
   T& front() { return vector_.front(); }
   T& back() { return vector_.back(); }

   const T& element(uint32_t _i) const { return vector_[_i]; }
   const T& front() const { return vector_.front(); }
   const T& back() const { return vector_.back(); }

   iterator elementDel(iterator it) { return vector_.erase(it); }
   void elementIs(uint32_t _i, const T& _e) { vector_[_i] = _e; }
   void pushBack(const T& _e) { vector_.push_back(_e); }
   void popBack() { vector_.pop_back(); }
   void clear() { vector_.clear(); }

   T& operator[](const uint32_t& _i) { return vector_[_i]; }
   const T& operator[](const uint32_t& _i) const { return vector_[_i]; }
protected:
private:
   // member functions =========================================================
   // data members =============================================================
   vector<T> vector_;
   
   /* operations disallowed */
   Vector(const Vector&);
   void operator=(const Vector&);
};

} /* end of namespace Simone */

#endif
