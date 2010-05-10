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

  /* forward declaration */
  class Query;

  /* nested class table row */
  class Row: public Simone::PtrInterface<Row> {
    friend class SQLiteDB;
  public:
    typedef Simone::Ptr<const Row> PtrConst;

    enum ValueType { kInteger, kFloat, kText, kBlob, kNull };

    ValueType type(int column_index) const;

    double dbl(int column_index) const;
    int integer(int column_index) const;
    int64_t integer64(int column_index) const;
    string text(int column_index) const;

    int byteCount(int column_index) const;
    const void *blob(int column_index) const;

    int columnCount() const { return sqlite3_column_count(query_->stmt_); }
    bool valid() const { return valid_; }

  private:
    Row(Simone::Ptr<Query> _query);

    /* member functions */
    void verify_column_index(int column_index) const;
    void verify_numeric(int column_index) const;
    void verify_valid() const;
    void validIs(bool _status) { valid_ = _status; }

    /* data members */

    /* smart pointer to guarantee continued existence of
       sqlite3_stmt* in Query throughout this object's existence */
    Simone::Ptr<Query> query_;
    bool valid_;
  };

  /* nested class query */
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

  static Ptr SQLiteDBNew(const string& _db_name) {
    return new SQLiteDB(_db_name);
  }

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
