cmake_minimum_required(VERSION 3.10)
project(BORDER_PANE_C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED notcurses)

add_executable(borderpane src/main.c src/panes.c)

add_compile_definitions(_XOPEN_SOURCE=700)

target_link_libraries(borderpane PRIVATE ${NOTCURSES_LIBRARIES} PRIVATE m)
target_include_directories(borderpane PRIVATE ${NOTCURSES_INCLUDE_DIRS})

