cmake_minimum_required(VERSION 3.15)

project(proj1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Enable compile_commands.json (clang-tidy needs it)

# main code
add_executable(proj1 src/main.cpp)

# Compiler warnings
target_compile_options(proj1 PRIVATE
  $<$<CXX_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic -Wconversion>
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

option(ENABLE_WERROR "Treat warnings as errors" ON)
if(ENABLE_WERROR)
  target_compile_options(proj1 PRIVATE
    $<$<CXX_COMPILER_ID:Clang,GNU>:-Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
  )
endif()

