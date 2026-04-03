cmake_minimum_required(VERSION 3.10)
project(warehouse)

# Чтобы работал автокомплит в clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем Raylib в твоем Arch Linux
find_package(raylib REQUIRED)

# Создаем само приложение из файла main.cpp
add_executable(warehouse main.cpp)

# Подключаем Raylib к нашей программе
target_link_libraries(warehouse raylib)