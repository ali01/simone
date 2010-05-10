#include "sqlite_db.h"

/* lib includes */
#include <stdint.h>

/* stl includes */
#include <string>
using std::string;

/* simone includes */
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

/* SQLiteDB::Row */

SQLiteDB::Row::Row(Query::Ptr _query) : query_(_query), valid_(true) {}

SQLiteDB::Row::ValueType
SQLiteDB::Row::type(int column_index) const {
  verify_valid();
  verify_column_index(column_index);

  ValueType type;
  int sqlite_type = sqlite3_column_type(query_->stmt_, column_index);

  switch (sqlite_type) {
    case SQLITE_INTEGER:
      type = kInteger;
      break;
    case SQLITE_FLOAT:
      type = kFloat;
      break;
    case SQLITE_TEXT:
      type = kText;
      break;
    case SQLITE_BLOB:
      type = kBlob;
      break;
    case SQLITE_NULL:
      type = kNull;
      break;
    default:
      string msg = "sqlite3 value type not recognized";
      throw InvalidValueException(__FILE__, __LINE__, msg);
  }

  return type;
}

double
SQLiteDB::Row::dbl(int column_index) const {
  verify_valid();
  verify_column_index(column_index);
  verify_numeric(column_index);

  return sqlite3_column_double(query_->stmt_, column_index);
}

int
SQLiteDB::Row::integer(int column_index) const {
  verify_valid();
  verify_column_index(column_index);
  verify_numeric(column_index);

  return sqlite3_column_int(query_->stmt_, column_index);
}

int64_t
SQLiteDB::Row::integer64(int column_index) const {
  verify_valid();
  verify_column_index(column_index);
  verify_numeric(column_index);

  return sqlite3_column_int64(query_->stmt_, column_index);
}

string
SQLiteDB::Row::text(int column_index) const {
  verify_valid();
  verify_column_index(column_index);

  ValueType tp = type(column_index);
  if (tp == kNull || tp == kBlob) {
    string msg = "sqlite3 value type unexpected";
    throw UnknownTypeException(__FILE__, __LINE__, msg);
  }
  
  const unsigned char *text = sqlite3_column_text(query_->stmt_, column_index);
  return string((const char*)text);
}

int
SQLiteDB::Row::byteCount(int column_index) const {
  verify_valid();
  verify_column_index(column_index);

  ValueType tp = type(column_index);
  if (tp != kBlob && tp != kText) {
    string msg = "sqlite3 value type unexpected";
    throw UnknownTypeException(__FILE__, __LINE__, msg);
  }

  return sqlite3_column_bytes(query_->stmt_, column_index);
}

const void *
SQLiteDB::Row::blob(int column_index) const {
  verify_valid();
  verify_column_index(column_index);

  ValueType tp = type(column_index);
  if (tp != kBlob) {
    string msg = "sqlite3 value type unexpected";
    throw UnknownTypeException(__FILE__, __LINE__, msg);
  }

  if (byteCount(column_index) <= 0) {
    string  msg = "sqlite zero length blob";
    throw UnknownTypeException(__FILE__, __LINE__, msg);
  }

  return sqlite3_column_blob(query_->stmt_, column_index);
}

void
SQLiteDB::Row::verify_column_index(int column_index) const {
  if (column_index >= columnCount()) {
    string msg = "sqlite3 column index out of range";
    throw RangeException(__FILE__, __LINE__, msg);
  }
}

void
SQLiteDB::Row::verify_numeric(int column_index) const {
  ValueType tp = type(column_index);
  if (tp != kInteger && tp != kFloat && tp != kNull) {
    string msg = "sqlite3 value type unexpected";
    throw UnknownTypeException(__FILE__, __LINE__, msg);
  }
}

void
SQLiteDB::Row::verify_valid() const {
  if (!valid_) {
    string msg = "operation on invalid sqlite3 table row";
    throw InvalidOperationException(__FILE__, __LINE__, msg);
  }
}

/* SQLiteDB::Query */

SQLiteDB::Query::Query(sqlite3 *_db, const string& _q) {
  int ret = sqlite3_prepare_v2(_db, _q.c_str(), _q.length() + 1, &stmt_, NULL);
  if (ret != SQLITE_OK) {
    string msg = sqlite3_errmsg(_db);
    throw StorageException(__FILE__, __LINE__, msg);
  }
}

void
SQLiteDB::Query::execute() {
  int ret;

  do {
    ret = sqlite3_step(stmt_);
  } while (ret == SQLITE_ROW);

  if (ret != SQLITE_DONE) {
    string msg = "error executing sqlite query";
    throw StorageException(__FILE__, __LINE__, msg);
  }
}

SQLiteDB::Row::PtrConst
SQLiteDB::Query::executeStep() {  
  int ret = sqlite3_step(stmt_);

  if (ret == SQLITE_ROW) {
    if (row_ == NULL)
      row_ = new Row(this);
  } else if (ret == SQLITE_DONE) {
    if (row_ != NULL) {
      row_->validIs(false);
      row_ = NULL;
    }  
  } else {
    string msg = "error executing sqlite query";
    throw StorageException(__FILE__, __LINE__, msg);
  }

  return row_;
}

} /* end of namespace S */
