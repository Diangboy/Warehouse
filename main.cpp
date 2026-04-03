#include <string>
#include <vector>

#include "raylib.h"

using namespace std;

struct Product {
  string name;
  int count;
};

int main() {
  InitWindow(800, 600, "Склад 2.0");
  SetTargetFPS(60);

  // 1. Указываем диапазон символов (латиница + кириллица)
  int codepoints[512];
  for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;           // Латиница
  for (int i = 0; i < 255; i++) codepoints[95 + i] = 0x400 + i;  // Кириллица

  // 2. Пробуем загрузить шрифт (Liberation - стандарт для Arch/Linux)
  Font font =
      LoadFontEx("/usr/share/fonts/liberation/LiberationSans-Regular.ttf", 32,
                 codepoints, 512);

  vector<Product> inventory = {{"Яблоки", 100}, {"Груши", 50}, {"Бананы", 200}};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextEx(font, "Мой Склад", {20, 20}, 32, 2, DARKGRAY);

    // Выводим список товаров
    for (int i = 0; i < inventory.size(); i++) {
      string info =
          inventory[i].name + ": " + to_string(inventory[i].count) + " шт.";
      DrawTextEx(font, info.c_str(), {20, (float)80 + i * 40}, 24, 2, BLACK);
    }

    DrawTextEx(font, "Нажми ESC для выхода", {20, 550}, 18, 2, LIGHTGRAY);
    EndDrawing();
  }

  UnloadFont(font);
  CloseWindow();
  return 0;
}
