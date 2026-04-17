#include "warehouse.h"
using namespace std;

bool IsButtonClicked(Rectangle rect) {
  return CheckCollisionPointRec(GetMousePosition(), rect) &&
         IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void DrawProductRow(Font font, Product& product, float yPos, int index,
                    vector<Product>& inventory) {
  string info = product.name + ": " + to_string(product.count);
  DrawTextEx(font, info.c_str(), {20, yPos}, 24, 2, BLACK);

  // Кнопки
  Rectangle btnPlus = {300, yPos, 30, 30};
  Rectangle btnMinus = {340, yPos, 30, 30};
  Rectangle btnDelete = {380, yPos, 30, 30};

  DrawRectangleRec(btnPlus, LIGHTGRAY);
  DrawRectangleRec(btnMinus, LIGHTGRAY);
  DrawRectangleRec(btnDelete, RED);

  DrawText("+", btnPlus.x + 10, btnPlus.y + 5, 20, BLACK);
  DrawText("-", btnMinus.x + 12, btnMinus.y + 5, 20, BLACK);
  DrawText("X", btnDelete.x + 10, btnDelete.y + 5, 20, WHITE);

  if (IsButtonClicked(btnPlus)) product.count++;
  if (IsButtonClicked(btnMinus) && product.count > 0) product.count--;

  if (IsButtonClicked(btnDelete)) {
    inventory.erase(inventory.begin() + index);
  }
}