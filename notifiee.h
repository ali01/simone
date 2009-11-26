/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a 
   Modeling & Simulation's Perspective ~ Chapter 3: Events, Notifications, and
   Callbacks. */
#ifndef BASENOTIFIEE_H_SZV7FZSO
#define BASENOTIFIEE_H_SZV7FZSO

#include "ptr.h"
#include "utility.h"
#include "exception.h"

namespace Simone {

template <typename Notifier>
class BaseNotifiee : public PtrInterface<BaseNotifiee<Notifier> >,
                     private boost::noncopyable {
public:
   typedef Simone::Ptr<const BaseNotifiee<Notifier> > PtrConst;
   typedef Simone::Ptr<BaseNotifiee<Notifier> > Ptr;
   
   virtual ~BaseNotifiee() {
      if (notifier_) {
         notifier_->notifieeDel(static_cast<typename Notifier::Notifiee*>(this));
         if ( ! stronglyReferencing()) { notifier_->newRef(); }
      }
   }
   
   typename Notifier::PtrConst notifier() const { return notifier_; }
   typename Notifier::Ptr      notifier()       { return notifier_; }
   
   bool stronglyReferencing() const { return strongly_ref_; }
   
   void notifierIs(const typename Notifier::Ptr& _n) {
      if (notifier_ == _n) { return; }
      if (notifier_) {
         if ( ! stronglyReferencing()) { notifier_->newRef(); }
         notifier_->notifieeDel(static_cast<typename Notifier::Notifiee*>(this));
         notifier_ = 0;
      }
      if (_n) {
         notifier_ = _n;
         notifier_->notifieeIs(static_cast<typename Notifier::Notifiee*>(this));
         if ( ! stronglyReferencing()) { notifier_->deleteRef(); }
      }
   }
   
   void stronglyReferencingIs(bool _s) {
      if(stronglyReferencing() == _s) { return; }
      strongly_ref_ = _s;
      if (notifier_) {
         if (stronglyReferencing()) { notifier_->newRef();    }
         else                       { notifier_->deleteRef(); }
      }
   }
   
   // supported notifications -----------------------------------------------------
protected:
   BaseNotifiee() {}
   typename Notifier::Ptr notifier_;
   bool                   strongly_ref_;
};

} /* end of namespace Simone */

#endif
