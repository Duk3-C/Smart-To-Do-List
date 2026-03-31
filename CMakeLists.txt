cmake_minimum_required(VERSION 3.10)
project(STODO_List)

find_package(nlohmann_json REQUIRED)

add_executable(STODO_List main.cpp)

target_link_libraries(STODO_List PRIVATE nlohmann_json::nlohmann_json)
