/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */
/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a
  Modeling & Simulation's Perspective ~ Chapter 3: Events, Notifications, and
  Callbacks. */
#ifndef BASENOTIFIEE_H_SZV7FZSO
#define BASENOTIFIEE_H_SZV7FZSO

#include "utility.h"
#include "exception.h"

#include "thread/concurrent_ptr_interface.h"

namespace Simone {

template <typename Notifier,
       typename ChildNotifiee=class Notifier::Notifiee>
class BaseNotifiee
 : public thread::ConcurrentPtrInterface<BaseNotifiee<Notifier,ChildNotifiee> >,
   private boost::noncopyable {
protected:
  BaseNotifiee() : strongly_ref_(true) {}
  virtual ~BaseNotifiee() {
    if (notifier_) {
      notifier_->notifieeDel(static_cast<ChildNotifiee*>(this));
      if ( ! stronglyReferencing()) { notifier_->newRef(); }
    }
  }

  typename Notifier::Ptr notifier_;
  bool              strongly_ref_;

public:

  typename Notifier::PtrConst notifier() const {
    return notifier_;
  }

  typename Notifier::Ptr notifier() {
    return notifier_;
  }

  void notifierIs(const typename Notifier::Ptr& _n) {
    if (notifier_ == _n) { return; }
    if (notifier_) {
      if ( ! stronglyReferencing()) { notifier_->newRef(); }
      notifier_->notifieeDel(static_cast<ChildNotifiee*>(this));
      notifier_ = NULL;
    }
    if (_n) {
      notifier_ = _n;
      notifier_->notifieeIs(static_cast<ChildNotifiee*>(this));
      if ( ! stronglyReferencing()) { notifier_->deleteRef(); }
    }
  }

  bool stronglyReferencing() const {
    return strongly_ref_;
  }

  void stronglyReferencingIs(bool _s) {
    if(stronglyReferencing() == _s) { return; }
    strongly_ref_ = _s;
    if (notifier_) {
      if (stronglyReferencing()) { notifier_->newRef();   }
      else                { notifier_->deleteRef(); }
    }
  }
};

} /* end of namespace Simone */

#endif
