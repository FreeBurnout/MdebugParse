cmake_minimum_required(VERSION 3.10)
project(MdebugParse)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/ELFIO)

set(SOURCE_FILES
        main.cpp)

add_executable(MdebugParse ${SOURCE_FILES})
target_link_libraries(MdebugParse elfio)