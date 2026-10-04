cmake_minimum_required(VERSION 3.18)
project(RAYCASTER_C)

find_package(SDL3 REQUIRED)

add_executable(raycaster)
target_sources(raycaster PRIVATE src/main.c src/render.c)

target_include_directories(raycaster PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(raycaster PRIVATE SDL3::SDL3 PRIVATE m)


