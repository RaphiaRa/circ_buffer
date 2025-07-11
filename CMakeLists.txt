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
    include(CMakePushCheckState)
    include(CheckCXXCompilerFlag)
    cmake_push_check_state(RESET)
    set(CMAKE_REQUIRED_LINK_OPTIONS -fsanitize=address)
    check_cxx_compiler_flag("-fsanitize=address" WITH_ASAN)
    cmake_pop_check_state()

    add_subdirectory(external/catch2)
    add_executable(Test
      test/test_main.cpp
      test/test_circ_buffer.cpp
    )
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
      target_compile_options(Test PRIVATE -Wall -Wextra -Wpedantic -Werror -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion)
    endif()

    if (WITH_ASAN)
      target_compile_options(Test PRIVATE -fsanitize=address)
      target_link_options(Test PRIVATE -fsanitize=address)
    endif()
  
    target_link_libraries(Test
      CircBuffer::CircBuffer
      Catch2::Catch2
    )
endif()
