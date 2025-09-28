cmake_minimum_required(VERSION 4.0)

project (hello)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compilers to use (9.2)
# set(CMAKE_C_COMPILER /opt/homebrew/Cellar/gcc/15.1.0/bin/gcc-15)
# set(CMAKE_CXX_COMPILER  /opt/homebrew/Cellar/gcc/15.1.0/bin/g++-15)
# Set the C and C++ compilers to Clang
# Use Homebrew LLVM Clang
#set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)
#set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)





set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules)

# openmp flags
#SET(OPENMP_CXX_FLAGS "-O3 -fopenmp --target=x86_64-pc-linux-gnu")
#SET(OPENMP_CXX_FLAGS "-O3 -fopenmp")
SET(OPENMP_CXX_FLAGS "") # blank flag for now
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OPENMP_CXX_FLAGS}" )

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
  target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
endif()

# app targets
include_directories(include)

#add_executable(main source/main.cpp source/Facade.cpp source/CsvParser.cpp)
add_executable(main source/main.cpp source/Facade.cpp source/CsvParser.cpp source/Stopwatch.cpp source/WorldDataParser.cpp)




