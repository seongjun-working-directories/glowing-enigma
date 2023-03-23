cmake_minimum_required(VERSION 3.0.2)
project(joycontroller)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  roscpp
  std_msgs
  tf
  rosbag
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES joycontroller
#  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp std_msgs tf rosbag
#  DEPENDS system_lib
)

include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

add_executable(devjoycontroller src/DevJoyController_Main.cpp src/DevJoyController_Function.cpp)
target_link_libraries(devjoycontroller ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})