#ifndef CONTROLLER_CPP_LIB_INTRUSIVE_PTR_INTERFACE_
#define CONTROLLER_CPP_LIB_INTRUSIVE_PTR_INTERFACE_

#include <boost/intrusive_ptr.hpp>

namespace controller {

// -- Forward declarations --
// TODO: comment

template <typename T>
void intrusive_ptr_add_ref(T* p);

template <typename T>
void intrusive_ptr_release(T* p);


// -- IntrusivePtrInterface --

class IntrusivePtrInterface {
 protected:
  IntrusivePtrInterface() : refs_(0) {}
  virtual ~IntrusivePtrInterface() {}

  // TODO: comment
  virtual void on_zero_refs() const { delete this; }

 private:
  template <typename T>
  friend void intrusive_ptr_add_ref(T* p);

  template <typename T>
  friend void intrusive_ptr_release(T* p);

  mutable unsigned long refs_;
};


// -- Implementation of inline functions --

template<typename T>
void
intrusive_ptr_add_ref(T* p) {
  // TODO: comment - will not be called if P is NULL.
  p->refs_ += 1;
}

template <typename T>
void
intrusive_ptr_release(T* p) {
  p->refs_ -= 1;
  if (p->refs_ == 0)
    p->on_zero_refs();
}

}  // namespace controller

#endif  // CONTROLLER_CPP_LIB_INTRUSIVE_PTR_INTERFACE_
