#include "sqlite_db.h"

#include "../exception.h"

namespace Simone {

SQLiteDB::SQLiteDB(const string& _db_name) {
  int flags = SQLITE_OPEN_READWRITE |
              SQLITE_OPEN_CREATE |
              SQLITE_OPEN_NOMUTEX;
  int ret = sqlite3_open_v2(_db_name.c_str(), &db, flags, NULL);
  if (ret != SQLITE_OK) {
    string msg = sqlite3_errmsg(db);
    throw StorageException(__FILE__, __LINE__, msg);
  }
}

SQLiteDB::~SQLiteDB() {
  sqlite3_close(db);
}

} /* end of namespace S */
