/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a
  Modeling & Simulation's Perspective ~ Chapter 5: Memory management with smart
  pointers. */

#ifndef PTR_H_M10ZP6RI
#define PTR_H_M10ZP6RI

#include <cassert>

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

  template <class OtherType>
  operator Ptr<OtherType>() const { return Ptr<OtherType>(ptr_); }

  struct PointerConversion { int valid; };
  operator int PointerConversion::*() const {
    return ptr_ ? &PointerConversion::valid : 0;
  }

  template <typename TargetS, typename SourceS>
  static Ptr<TargetS> st_cast(Ptr<SourceS> _o) {
    return static_cast<TargetS*>(_o.ptr());
  }
  
  template <typename TargetD, typename SourceD>
  static Ptr<TargetD> dyn_cast(Ptr<SourceD> _o) {
    return dynamic_cast<TargetD*>(_o.ptr());
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

}

#endif
