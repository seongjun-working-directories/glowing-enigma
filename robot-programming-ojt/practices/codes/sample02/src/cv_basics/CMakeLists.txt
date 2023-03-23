cmake_minimum_required(VERSION 3.0.2)
project(cv_basics)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  std_msgs
)
find_package(OpenCV REQUIRED)

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
 INCLUDE_DIRS include
 CATKIN_DEPENDS cv_bridge image_transport roscpp rospy sensor_msgs std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(hello src/hello.cpp)
target_link_libraries(hello ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

# add_executable(roscpp_opencv_example src/roscpp_opencv_example.cpp)
# target_link_libraries(roscpp_opencv_example ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})