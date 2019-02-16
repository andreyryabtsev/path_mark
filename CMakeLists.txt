cmake_minimum_required(VERSION 2.8)
project(path_mark)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Eigen3 REQUIRED)

include_directories(
    include
    ${EIGEN3_INCLUDE_DIR}
)

add_library(${PROJECT_NAME}
    src/Robot.cc
    src/NLinkArm.h
    src/World.cc
    src/client.cc
)

add_executable(client src/client.cc)
target_link_libraries(client #{PROJECT_NAME}
    )
