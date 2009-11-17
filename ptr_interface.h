/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a 
   Modeling & Simulation's Perspective ~ Chapter 5: Memory management with smart
   pointers. */

#ifndef PTRINTERFACE_H_VGPMQRTI
#define PTRINTERFACE_H_VGPMQRTI
#include "../globals.h"

namespace Simone {

template <class T>
class PtrInterface {
   friend class Ptr<T>;
public:
    PtrInterface() : ref_(0) {}
    unsigned long references() const { return ref_; }
    // DRC - support for templates
    inline const PtrInterface * newRef() const { ++ref_; return this; }
    inline void deleteRef() const { if( --ref_ == 0 ) onZeroReferences(); }
    inline void referencesDec(uint32_t dec) const
                                { if((ref_ -= dec) <= 0 ) onZeroReferences(); }
protected:
    virtual ~PtrInterface() {}
    virtual void onZeroReferences() const { delete this; }
private:
    mutable long unsigned ref_;
};

}

#endif
