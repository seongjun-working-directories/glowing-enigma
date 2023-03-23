cmake_minimum_required(VERSION 3.0.2)
project(turtle_to_servinggo)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  std_msgs
  turtlesim
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES turtle_to_servinggo
#  CATKIN_DEPENDS geometry_msgs roscpp std_msgs turtlesim
#  DEPENDS system_lib
)

include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

add_executable(turtle_to_servinggo_node src/Main.cpp src/Recorder.cpp src/Replayer.cpp src/Player.cpp)
target_link_libraries(turtle_to_servinggo_node ${catkin_LIBRARIES})