#include "database.h"

#include <iostream>

using namespace std;

bool RegisterUser(string username, string password) {
  sqlite3* db;
  sqlite3_open("warehouse.db", &db);

  string sql = "INSERT INTO users (username, password) VALUES (?, ?);";
  sqlite3_stmt* stmt;

  sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return (rc == SQLITE_DONE);
}

bool LoginUser(string username, string password) {
  sqlite3* db;
  sqlite3_open("warehouse.db", &db);

  string sql = "SELECT * FROM users WHERE username = ? AND password = ?;";
  sqlite3_stmt* stmt;

  sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

  int rc = sqlite3_step(stmt);
  bool exists = (rc == SQLITE_ROW);

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return exists;
}

bool InitDatabase() {
  sqlite3* db;
  int rc = sqlite3_open("warehouse.db", &db);

  if (rc != SQLITE_OK) {
    cerr << "Не удалось открыть БД: " << sqlite3_errmsg(db) << endl;
    return false;
  }

  const char* sql =
      "CREATE TABLE IF NOT EXISTS users ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "username TEXT UNIQUE NOT NULL,"
      "password TEXT NOT NULL);";

  char* errMsg = 0;
  rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

  if (rc != SQLITE_OK) {
    cerr << "Ошибка SQL: " << errMsg << endl;
    sqlite3_free(errMsg);
    sqlite3_close(db);
    return false;
  }

  sqlite3_close(db);
  return true;
}