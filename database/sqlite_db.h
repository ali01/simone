#ifndef SQLITE_DB_H_GDKPNVR1
#define SQLITE_DB_H_GDKPNVR1

/* sqlite includes */
#include <sqlite3.h>

/* simone includes */
#include "../ptr_interface.h"

namespace Simone {

class SQLiteDB : public Simone::PtrInterface<SQLiteDB> {
public:
  typedef Simone::Ptr<const SQLiteDB> PtrConst;
  typedef Simone::Ptr<SQLiteDB> Ptr;
  
  static Ptr SQLiteDBNew(const string& _db_name) {
    return new SQLiteDB(_db_name);
  }
  
  class Query : public Simone::PtrInterface<Query> {
    friend class SQLiteDB;
  public:
    typedef Simone::Ptr<const Query> PtrConst;
    typedef Simone::Ptr<Query> Ptr;
    
  private:
    Query(sqlite3 *_db, const string& _query);
    
    /* data members */
    sqlite3_stmt *stmt_;
  };

  Query::PtrConst query(const string & _query) const {
    return new Query(db_, _query);
  }

private:
  SQLiteDB(const string& _db_name);
  virtual ~SQLiteDB();
  
  /* data members */
  sqlite3 *db_;
};

} /* end of namespace Simone */

#endif
