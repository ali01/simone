/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a 
   Modeling & Simulation's Perspective ~ Chapter 3: Events, Notifications, and
   Callbacks. */
#ifndef BASENOTIFIEE_H_SZV7FZSO
#define BASENOTIFIEE_H_SZV7FZSO

#include "ptr.h"
#include "utility.h"
#include "exception.h"

#include "thread/scoped_lock.h"
#include "thread/recursive_mutex.h"
using Simone::thread::RecursiveMutex;
using Simone::thread::ScopedLock;

namespace Simone {

template <typename Notifier,
          typename ChildNotifiee=class Notifier::Notifiee,
          bool _thread_safe_=false>
class BaseNotifiee :
            public PtrInterface<BaseNotifiee<Notifier,ChildNotifiee,_thread_safe_> >,
            private boost::noncopyable {
protected:
   BaseNotifiee() : strongly_ref_(true) {}
   virtual ~BaseNotifiee() {
      if (_thread_safe_) { ScopedLock lk(mutex_); };
      if (notifier_) {
         notifier_->notifieeDel(static_cast<ChildNotifiee*>(this));
         if ( ! stronglyReferencing()) { notifier_->newRef(); }
      }
   }
   
   typename Notifier::Ptr notifier_;
   bool                   strongly_ref_;
   
   mutable RecursiveMutex mutex_;
   
public:
   typedef Simone::Ptr<const BaseNotifiee<Notifier,ChildNotifiee,_thread_safe_> >
                                                                             PtrConst;
   typedef Simone::Ptr<BaseNotifiee<Notifier,ChildNotifiee,_thread_safe_> > Ptr;
   
   typename Notifier::PtrConst notifier() const {
      if (_thread_safe_) { ScopedLock lk(mutex_); };
      return notifier_;
   }
   
   typename Notifier::Ptr notifier() {
      if (_thread_safe_) { ScopedLock lk(mutex_); };
      return notifier_;
   }
   
   void notifierIs(const typename Notifier::Ptr& _n) {
      if (_thread_safe_) { ScopedLock lk(mutex_); };
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
      if (_thread_safe_) { ScopedLock lk(mutex_); };
      return strongly_ref_;
   }
   
   void stronglyReferencingIs(bool _s) {
      if (_thread_safe_) { ScopedLock lk(mutex_); };
      if(stronglyReferencing() == _s) { return; }
      strongly_ref_ = _s;
      if (notifier_) {
         if (stronglyReferencing()) { notifier_->newRef();    }
         else                       { notifier_->deleteRef(); }
      }
   }
};

} /* end of namespace Simone */

#endif
