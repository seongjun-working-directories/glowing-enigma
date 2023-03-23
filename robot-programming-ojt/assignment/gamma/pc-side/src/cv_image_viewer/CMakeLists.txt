cmake_minimum_required(VERSION 3.0.2)
project(cv_image_viewer)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
 LIBRARIES cv_image_viewer
 CATKIN_DEPENDS cv_bridge image_transport roscpp rospy sensor_msgs std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(image_viewing src/image_viewing.cpp)
target_link_libraries(image_viewing ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(compressed_image_viewing src/compressed_image_viewing.cpp)
target_link_libraries(compressed_image_viewing ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(decoded_compressed_image_viewing src/decoded_compressed_image_viewing.cpp)
target_link_libraries(decoded_compressed_image_viewing ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
