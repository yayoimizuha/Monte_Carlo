cmake_minimum_required(VERSION 3.23)
project(Monte_Carlo)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)
if (OpenMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()

add_executable(Monte_Carlo_1 Monte_Carlo_1.cpp)
add_executable(Monte_Carlo_2 Monte_Carlo_2.cpp)
add_executable(Monte_Carlo_3 Monte_Carlo_3.cpp)
add_executable(Monte_Carlo_4 Monte_Carlo_4.cpp)
add_executable(Monte_Carlo_5 Monte_Carlo_5.cpp)
