cmake_minimum_required(VERSION 2.8)
project(path_mark)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pthread")

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Eigen3 REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(
    include
    ${EIGEN3_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
    src/Robot.h
    src/NLinkArm.h
    src/World.cc
    src/Visualizer.cc
    src/Renderer.cc
    src/Drawable.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

add_executable(client src/client.cc)
target_link_libraries(client ${PROJECT_NAME})

add_executable(gls_test src/gls_test.cc)
target_link_libraries(gls_test ${PROJECT_NAME})# ${OpenCV_LIBS})
