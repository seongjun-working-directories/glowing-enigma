cmake_minimum_required(VERSION 3.0.2)
project(pcl2_controller)
add_compile_options(-std=c++14)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  geometry_msgs
  std_msgs
  pcl_ros
  pcl_conversions
  laser_geometry
)

find_package(OpenCV REQUIRED)
find_package(PCL 1.10 REQUIRED)
find_package(Boost REQUIRED)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   sensor_msgs#   std_msgs
# )

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES pcl2_controller
 CATKIN_DEPENDS cv_bridge image_transport roscpp rospy sensor_msgs geometry_msgs std_msgs pcl_ros pcl_conversions laser_geometry
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(pcl2_controller_main src/pcl2_controller_main.cpp)
target_link_libraries(pcl2_controller_main ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
