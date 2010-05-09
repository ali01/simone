#include "sqlite_db.h"

#include "../exception.h"

namespace Simone {

SQLiteDB::SQLiteDB(const string& _db_name) {
  /* - OPEN_CREATE creates database if it doesn't already exist;
     - OPEN_NOMUTEX allows multithreaded access to database as long as any
                    single sqlite3* abject is always accessed by one thread; */
  int flags = SQLITE_OPEN_READWRITE |
              SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX;
  
  /* initialize database using open_v2(); open() is deprecated */
  int ret = sqlite3_open_v2(_db_name.c_str(), &db_, flags, NULL);
  if (ret != SQLITE_OK) {
    string msg = sqlite3_errmsg(db_);
    throw StorageException(__FILE__, __LINE__, msg);
  }
}

SQLiteDB::~SQLiteDB() {
  sqlite3_close(db_);
}

SQLiteDB::Query::Query(sqlite3 *_db, const string& _q) {
  int ret = sqlite3_prepare_v2(_db, _q.c_str(), _q.length() + 1, &stmt_, NULL);
  if (ret != SQLITE_OK) {
    string msg = sqlite3_errmsg(_db);
    throw StorageException(__FILE__, __LINE__, msg);
  }
}

} /* end of namespace S */
