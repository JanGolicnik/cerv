cmake_minimum_required(VERSION 3.10)

project(cerv)

set(CMAKE_C_STANDARD 11)

set(SOURCES main.c)

add_executable(cerv ${SOURCES})
