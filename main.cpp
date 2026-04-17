#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "database.h"
#include "raylib.h"
#include "warehouse.h"

enum AppState { LOGIN_SCREEN, WAREHOUSE_SCREEN };

int main() {
  if (!InitDatabase()) {
    std::cerr << "Критическая ошибка: База данных недоступна!" << std::endl;
    return -1;
  }

  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Warehouse System 2.0");
  SetTargetFPS(60);

  AppState currentState = LOGIN_SCREEN;
  char loginInput[32] = {0};
  char passwordInput[32] = {0};
  int loginLetterCount = 0;
  int passLetterCount = 0;
  bool editingPassword = false;

  Font font =
      LoadFontEx("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf", 32, 0, 512);

  while (!WindowShouldClose()) {
    if (currentState == LOGIN_SCREEN) {
      if (IsKeyPressed(KEY_TAB)) editingPassword = !editingPassword;

      int key = GetCharPressed();
      while (key > 0) {
        if ((key >= 32) && (key <= 125)) {
          if (!editingPassword && loginLetterCount < 31) {
            loginInput[loginLetterCount] = (char)key;
            loginInput[loginLetterCount + 1] = '\0';
            loginLetterCount++;
          } else if (editingPassword && passLetterCount < 31) {
            passwordInput[passLetterCount] = (char)key;
            passwordInput[passLetterCount + 1] = '\0';
            passLetterCount++;
          }
        }
        key = GetCharPressed();
      }

      if (IsKeyPressed(KEY_BACKSPACE)) {
        if (!editingPassword && loginLetterCount > 0) {
          loginLetterCount--;
          loginInput[loginLetterCount] = '\0';
        } else if (editingPassword && passLetterCount > 0) {
          passLetterCount--;
          passwordInput[passLetterCount] = '\0';
        }
      }

      if (IsKeyPressed(KEY_ENTER)) {
        if (LoginUser(loginInput, passwordInput)) {
          currentState = WAREHOUSE_SCREEN;
        } else {
          std::cout << "Ошибка: Неверные данные!" << std::endl;
          // Очистим пароль при ошибке
          memset(passwordInput, 0, sizeof(passwordInput));
          passLetterCount = 0;
        }
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (currentState == LOGIN_SCREEN) {
      DrawTextEx(font, "АВТОРИЗАЦИЯ", {280, 100}, 40, 2, DARKGRAY);

      DrawTextEx(font, "Логин:", {200, 200}, 24, 2, GRAY);
      DrawRectangleLines(200, 230, 400, 40,
                         !editingPassword ? BLUE : LIGHTGRAY);
      DrawTextEx(font, loginInput, {210, 235}, 28, 2, BLACK);

      DrawTextEx(font, "Пароль:", {200, 300}, 24, 2, GRAY);
      DrawRectangleLines(200, 330, 400, 40, editingPassword ? BLUE : LIGHTGRAY);

      std::string stars(passLetterCount, '*');
      DrawTextEx(font, stars.c_str(), {210, 345}, 28, 2, BLACK);

      DrawTextEx(font, "TAB - смена поля | ENTER - войти", {200, 400}, 18, 2,
                 DARKGRAY);
    } else if (currentState == WAREHOUSE_SCREEN) {
      DrawTextEx(font, "Управление складом", {20, 20}, 32, 2, DARKGRAY);

      DrawTextEx(font, "Вы успешно вошли!", {20, 100}, 24, 2, GREEN);
      DrawTextEx(font, "Нажмите ESC для выхода", {20, 550}, 18, 2, GRAY);
    }

    EndDrawing();
  }

  UnloadFont(font);
  CloseWindow();
  return 0;
}