cmake_minimum_required(VERSION 3.10)
project(warehouse)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(warehouse main.cpp warehouse.cpp database.cpp)

target_link_libraries(warehouse PRIVATE 
    raylib 
    SQLite3::SQLite3
)