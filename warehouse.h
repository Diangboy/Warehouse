#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <vector>

#include "raylib.h"
using namespace std;

struct Product {
  string name;
  int count;
};

void DrawProductRow(Font font, Product& product, float yPos, int index,
                    vector<Product>& inventory);
bool IsButtonClicked(Rectangle rect);

#endif