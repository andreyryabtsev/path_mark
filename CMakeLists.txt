cmake_minimum_required(VERSION 2.8)
project(path_mark)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pthread")

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Eigen3 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(gls REQUIRED)
find_package(catkin REQUIRED)
find_package(ompl REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread filesystem graph program_options system)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OMPL_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${EIGEN3_INCLUDE_DIR}
  ${gls_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)

link_libraries(
  ${catkin_LIBRARIES}
  ${gls_LIBRARIES}
  ${OMPL_LIBRARIES}
  ${Boost_LIBRARIES}
)

add_library(${PROJECT_NAME}
    src/Robot.h
    src/NLinkArm.h
    src/World.cc
    src/Visualizer.cc
    src/Renderer.cc
    src/Drawable.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${gls_LIBRARIES} ${ompl})

add_executable(client src/client.cc)
target_link_libraries(client ${PROJECT_NAME})

add_executable(gls_test src/gls_test.cc)
target_link_libraries(gls_test ${PROJECT_NAME})
