#ifndef MAP_H_LRU4H5ZH
#define MAP_H_LRU4H5ZH

#include <map>
#include "ptr.h"


namespace Simone {
using std::map;

template <typename KeyType, typename ValueType>
class Map : public PtrInterface<Map<KeyType,ValueType> > {
public:
   // type declarations ==============================================================
   typedef Simone::Ptr<const Map<KeyType,ValueType> > PtrConst;
   typedef Simone::Ptr<Map<KeyType,ValueType> > Ptr;
   
   typedef typename map<KeyType,ValueType>::iterator               iterator;
   typedef typename map<KeyType,ValueType>::reverse_iterator       reverse_iterator;
   typedef typename map<KeyType,ValueType>::const_iterator         const_iterator;
   typedef typename map<KeyType,ValueType>::const_reverse_iterator
                                                               const_reverse_iterator;
   
   // factory constructor ============================================================
   static Ptr MapNew() { return new Map(); }
   Map() {}
   
   // iterators ======================================================================
   iterator               begin()        { return map_.begin();  }
   iterator               end()          { return map_.end();    }
   reverse_iterator       rbegin()       { return map_.rbegin(); }
   reverse_iterator       rend()         { return map_.rend();   }
   
   const_iterator         begin()  const { return map_.begin();  }
   const_iterator         end()    const { return map_.end();    }
   const_reverse_iterator rbegin() const { return map_.rbegin(); }
   const_reverse_iterator rend()   const { return map_.rend();   }
   
   // accessors  =====================================================================
   size_t size()  const { return map_.size();  }
   bool   empty() const { return map_.empty(); }
   
   iterator       element(const KeyType& _key)       { return map_.find(_key); }
   const_iterator element(const KeyType& _key) const { return map_.find(_key); }
   
   // mutators =======================================================================
   void elementIs(const KeyType& _key, const ValueType& _v) { map_[_key] = _v; }
   void elementDel(iterator _it) { map_.erase(_it); }
   void elementDel(const KeyType& _key) { map_.erase(_key); }
   
         ValueType& operator[](const KeyType& key)       { return map_[key]; }
   const ValueType& operator[](const KeyType& key) const { return map_[key]; }
protected:
private:
   // member functions ===============================================================
   // data members ===================================================================
   map<KeyType,ValueType> map_;
   // disallowed operations ==========================================================
   Map(const Map&);
   void operator=(const Map&);
};

} /* end of namespace Simone */

#endif
