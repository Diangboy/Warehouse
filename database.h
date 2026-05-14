#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>

struct Product {
    int id;
    std::string name;
    int qty;
    float price;
};

void InitDatabase();
void AddProduct(int userId, const char* name, int qty, float price);
std::vector<Product> GetAllProducts(int userId);
void DeleteProduct(int id);

// Функции авторизации
int RegisterUser(const char* username, const char* password);
int LoginUser(const char* username, const char* password);

#endif