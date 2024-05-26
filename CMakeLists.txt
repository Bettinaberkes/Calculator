cmake_minimum_required(VERSION 3.10)
project(Calculator C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(calculator src/main.c src/calculator.c)
target_link_libraries(calculator pthread)
