#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

#include <string>

bool InitDatabase();
bool RegisterUser(std::string username, std::string password);
bool LoginUser(std::string username, std::string password);

#endif