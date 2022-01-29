cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(CircBuffer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

add_library(CircBuffer INTERFACE)
add_library(CircBuffer::CircBuffer ALIAS CircBuffer)

target_include_directories(CircBuffer
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (NOT DISABLE_TESTS)
    add_subdirectory(external/catch2)
    add_executable(Test
      test/test_main.cpp
      test/test_circ_buffer.cpp
    )
  
    target_link_libraries(Test
      CircBuffer::CircBuffer
      Catch2::Catch2
    )
endif()
