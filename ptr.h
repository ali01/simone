/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a
  Modeling & Simulation's Perspective ~ Chapter 5: Memory management with smart
  pointers. */

#ifndef PTR_H_M10ZP6RI
#define PTR_H_M10ZP6RI

#include <cassert>
#include <iostream>
#include <ios>
using std::hex;
using std::ostream;

namespace Simone {

template <typename T>
class Ptr {
public:
  Ptr () : ptr_(NULL) {}
  
  Ptr(T* p) : ptr_(p) {
    if (ptr_)
      ptr_->newRef();
  }

  Ptr(const Ptr<T>& mp) : ptr_(mp.ptr_) {
    if (ptr_)
      ptr_->newRef();
  }

  virtual ~Ptr() {
    if (ptr_) {
      if (ptr_->deleteRef() == NULL)
        ptr_ = NULL;
    }
  }

  Ptr<T>& operator=(const Ptr<T>& mp);
  Ptr<T>& operator=(Ptr<T>& mp);
  Ptr<T>& operator=(T* p);

  bool operator==(const Ptr<T>& mp) const { return ptr_ == mp.ptr_; }
  bool operator!=(const Ptr<T>& mp) const { return ptr_ != mp.ptr_; }
  bool operator==(T* p) const { return ptr_ == p; }
  bool operator!=(T* p) const { return ptr_ != p; }
  bool operator<(const Ptr<T>& mp) const { return ptr_ < mp.ptr_; }

  const T& operator*() const {
    assert(ptr_ != NULL);
    return *ptr_;
  }

  T& operator*() {
    assert(ptr_ != NULL);
    return *ptr_;
  }

  const T * operator->() const {
    assert(ptr_ != NULL);
    return ptr_;
  }

  T * operator->() {
    assert(ptr_ != NULL);
    return ptr_;
  }

  T * ptr() const { return ptr_; }

  /* cast operator -- other types */
  template <class OtherType>
  operator Ptr<OtherType>() const { return Ptr<OtherType>(ptr_); }

  template <typename TargetS, typename SourceS>
  static Ptr<TargetS> st_cast(Ptr<SourceS> _o) {
    return static_cast<TargetS*>(_o.ptr());
  }

protected:
  T *ptr_;
};

template<class T> Ptr<T>&
Ptr<T>::operator=(const Ptr<T>& mp) {
  const T * save = ptr_;
  ptr_ = mp.ptr_;

  if (ptr_)
    ptr_->newRef();

  if (save)
    save->deleteRef();
  
  return *this;
}

template<class T> Ptr<T>&
Ptr<T>::operator=(Ptr<T>& mp) {
  T * save = ptr_;
  ptr_ = mp.ptr_;

  if (ptr_)
    ptr_->newRef();

  if (save)
    save->deleteRef();

  return *this;
}

template<class T> Ptr<T>&
Ptr<T>::operator=(T* p) {
  T * save = ptr_;
  ptr_ = p;

  if (ptr_)
    ptr_->newRef();

  if (save)
    save->deleteRef();

  return *this;
}

template<class T>
ostream&
operator<<(ostream& out, const Ptr<T>& _s) {
  size_t size = 2 * sizeof(int*) + 4;
  char buf[size];
  snprintf(buf, size, "%p", _s.ptr());
  return out << buf;
}

}

#endif
