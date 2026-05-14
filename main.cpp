#include "database.h"
#include "raylib.h"
#include <cstring>
#include <string>
#include <vector>

enum AppState { STATE_LOGIN, STATE_REGISTER, STATE_WAREHOUSE };
AppState currentState = STATE_LOGIN;
int currentUserId = -1;

int main() {
  InitWindow(900, 650, "Warehouse System");
  SetTargetFPS(60);
  InitDatabase();

  Font font =
      LoadFontEx("/usr/share/fonts/TTF/DejaVuSans.ttf", 32, 0, 512);
  SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

  char authUser[32] = {0}, authPass[32] = {0};
  char prodName[32] = {0}, prodQty[16] = {0}, prodPrice[16] = {0};
  int activeField = 0;
  std::vector<Product> inventory;

  while (!WindowShouldClose()) {
    Vector2 mouse = GetMousePosition();

    // 1. Обработка ввода текста
    int key = GetCharPressed();
    while (key > 0) {
      char *target = nullptr;
      if (currentState != STATE_WAREHOUSE)
        target = (activeField == 0) ? authUser : authPass;
      else
        target = (activeField == 0)   ? prodName
                 : (activeField == 1) ? prodQty
                                      : prodPrice;

      if (target && strlen(target) < 30) {
        int len = strlen(target);
        target[len] = (char)key;
        target[len + 1] = '\0';
      }
      key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
      char *target = (currentState != STATE_WAREHOUSE)
                         ? ((activeField == 0) ? authUser : authPass)
                         : ((activeField == 0)   ? prodName
                            : (activeField == 1) ? prodQty
                                                 : prodPrice);
      if (target && strlen(target) > 0)
        target[strlen(target) - 1] = '\0';
    }

    if (IsKeyPressed(KEY_TAB))
      activeField = (currentState == STATE_WAREHOUSE) ? (activeField + 1) % 3
                                                      : (activeField + 1) % 2;

    BeginDrawing();
    ClearBackground({245, 245, 245, 255});

    if (currentState == STATE_LOGIN || currentState == STATE_REGISTER) {
      // ФОРМА АВТОРИЗАЦИИ
      DrawRectangle(275, 150, 350, 350, WHITE);
      DrawRectangleLines(275, 150, 350, 350, LIGHTGRAY);
      DrawTextEx(font, (currentState == STATE_LOGIN ? "LOGIN" : "REGISTER"),
                 {300, 180}, 24, 2, DARKGRAY);

      for (int i = 0; i < 2; i++) {
        Rectangle r = {300, (float)(250 + i * 80), 300, 40};
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mouse, r))
          activeField = i;
        DrawRectangleRec(r, (activeField == i) ? ColorAlpha(SKYBLUE, 0.1f)
                                               : (Color){240, 240, 240, 255});
        DrawRectangleLinesEx(r, 1, (activeField == i) ? SKYBLUE : LIGHTGRAY);

        if (i == 1) {
          std::string stars(strlen(authPass), '*');
          DrawTextEx(font, stars.c_str(), {310, 340}, 18, 1, BLACK);
        } else
          DrawTextEx(font, authUser, {310, 260}, 18, 1, BLACK);
      }

      Rectangle btn = {300, 410, 300, 45};
      bool hover = CheckCollisionPointRec(mouse, btn);
      DrawRectangleRec(btn, hover ? SKYBLUE : BLUE);
      DrawTextEx(font, "CONFIRM", {405, 422}, 18, 1, WHITE);

      if ((hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
          IsKeyPressed(KEY_ENTER)) {
        int res = (currentState == STATE_LOGIN)
                      ? LoginUser(authUser, authPass)
                      : RegisterUser(authUser, authPass);
        if (res != -1) {
          currentUserId = res;
          inventory = GetAllProducts(currentUserId);
          currentState = STATE_WAREHOUSE;
          activeField = 0;
        }
      }

      Rectangle switchRect = {300, 470, 300, 20};
      if (CheckCollisionPointRec(mouse, switchRect) &&
          IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState =
            (currentState == STATE_LOGIN) ? STATE_REGISTER : STATE_LOGIN;
        memset(authUser, 0, 32);
        memset(authPass, 0, 32);
      }
      DrawTextEx(font, "Switch Mode", {410, 470}, 14, 1, GRAY);

    } else {
      // ИНТЕРФЕЙС СКЛАДА
      DrawTextEx(font, "MY WAREHOUSE", {30, 25}, 26, 2, DARKGRAY);
      DrawLine(30, 65, 550, 65, LIGHTGRAY);
      DrawTextEx(font, "NAME", {40, 80}, 16, 2, GRAY);
      DrawTextEx(font, "QTY", {220, 80}, 16, 2, GRAY);
      DrawTextEx(font, "PRICE", {320, 80}, 16, 2, GRAY);

      float listY = 120;
      for (const auto &item : inventory) {
        DrawTextEx(font, item.name.c_str(), {40, listY}, 18, 1, BLACK);
        DrawTextEx(font, std::to_string(item.qty).c_str(), {220, listY}, 18, 1,
                   {0, 82, 172, 255});
        DrawTextEx(font, TextFormat("%.2f", item.price), {320, listY}, 18, 1,
                   {0, 121, 64, 255});

        Rectangle delBtn = {480, listY - 5, 60, 30};
        if (CheckCollisionPointRec(mouse, delBtn) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          DeleteProduct(item.id);
          inventory = GetAllProducts(currentUserId);
        }
        DrawRectangleRec(delBtn, RED);
        DrawTextEx(font, "DEL", {495, listY + 2}, 14, 1, WHITE);
        listY += 40;
      }

      // ПРАВАЯ ПАНЕЛЬ
      DrawRectangle(580, 0, 320, 650, WHITE);
      DrawLine(580, 0, 580, 650, LIGHTGRAY);
      DrawTextEx(font, "ADD PRODUCT", {600, 40}, 22, 2, DARKGRAY);

      const char *labels[] = {"Product Name:", "Quantity:", "Price ($):"};
      char *bufs[] = {prodName, prodQty, prodPrice};
      float fieldYs[] = {100, 180, 260};

      for (int i = 0; i < 3; i++) {
        DrawTextEx(font, labels[i], {600, fieldYs[i] - 20}, 14, 1, GRAY);
        Rectangle r = {600, fieldYs[i], 260, 40};
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mouse, r))
          activeField = i;
        DrawRectangleRec(r, (activeField == i) ? ColorAlpha(SKYBLUE, 0.1f)
                                               : (Color){240, 240, 240, 255});
        DrawRectangleLinesEx(r, 1, (activeField == i) ? SKYBLUE : LIGHTGRAY);
        DrawTextEx(font, bufs[i], {610, fieldYs[i] + 10}, 18, 1, BLACK);
      }

      Rectangle saveBtn = {600, 340, 260, 50};
      bool saveHover = CheckCollisionPointRec(mouse, saveBtn);
      if ((saveHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
          IsKeyPressed(KEY_ENTER)) {
        if (strlen(prodName) > 0) {
          AddProduct(currentUserId, prodName, atoi(prodQty),
                     (float)atof(prodPrice));
          inventory = GetAllProducts(currentUserId); // ОБНОВЛЕНИЕ ТУТ
          memset(prodName, 0, 32);
          memset(prodQty, 0, 16);
          memset(prodPrice, 0, 16);
          activeField = 0;
        }
      }
      DrawRectangleRec(saveBtn, saveHover ? LIME : GREEN);
      DrawTextEx(font, "SAVE", {705, 355}, 18, 1, WHITE);
    }

    EndDrawing();
  }

  UnloadFont(font);
  CloseWindow();
  return 0;
}