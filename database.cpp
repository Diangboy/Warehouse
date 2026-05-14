#include "database.h"
#include <sqlite3.h>
#include <cstdio>

void InitDatabase() {
    sqlite3* db;
    if (sqlite3_open("warehouse.db", &db) == SQLITE_OK) {
        const char* userTable = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT);";
        const char* prodTable = "CREATE TABLE IF NOT EXISTS products (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, name TEXT, quantity INTEGER, price REAL);";
        sqlite3_exec(db, userTable, 0, 0, 0);
        sqlite3_exec(db, prodTable, 0, 0, 0);
    }
    sqlite3_close(db);
}

int RegisterUser(const char* username, const char* password) {
    sqlite3* db;
    int lastId = -1;
    if (sqlite3_open("warehouse.db", &db) == SQLITE_OK) {
        sqlite3_stmt* stmt;
        const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_DONE) lastId = (int)sqlite3_last_insert_rowid(db);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    return lastId;
}

int LoginUser(const char* username, const char* password) {
    sqlite3* db;
    int userId = -1;
    if (sqlite3_open("warehouse.db", &db) == SQLITE_OK) {
        sqlite3_stmt* stmt;
        const char* sql = "SELECT id FROM users WHERE username = ? AND password = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) userId = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    return userId;
}

void AddProduct(int userId, const char* name, int qty, float price) {
    sqlite3* db;
    if (sqlite3_open("warehouse.db", &db) == SQLITE_OK) {
        sqlite3_stmt* stmt;
        const char* sql = "INSERT INTO products (user_id, name, quantity, price) VALUES (?, ?, ?, ?);";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, userId);
            sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, qty);
            sqlite3_bind_double(stmt, 4, price);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
}

std::vector<Product> GetAllProducts(int userId) {
    std::vector<Product> products;
    sqlite3* db;
    if (sqlite3_open("warehouse.db", &db) == SQLITE_OK) {
        sqlite3_stmt* stmt;
        const char* sql = "SELECT id, name, quantity, price FROM products WHERE user_id = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, userId);
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                products.push_back({
                    sqlite3_column_int(stmt, 0),
                    (const char*)sqlite3_column_text(stmt, 1),
                    sqlite3_column_int(stmt, 2),
                    (float)sqlite3_column_double(stmt, 3)
                });
            }
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    return products;
}

void DeleteProduct(int id) {
    sqlite3* db;
    if (sqlite3_open("warehouse.db", &db) == SQLITE_OK) {
        sqlite3_stmt* stmt;
        const char* sql = "DELETE FROM products WHERE id = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
}