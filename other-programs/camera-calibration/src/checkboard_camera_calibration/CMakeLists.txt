cmake_minimum_required(VERSION 3.0.2)
project(checkboard_camera_calibration)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

find_package(OpenCV REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES checkboard_camera_calibration
 CATKIN_DEPENDS cv_bridge image_transport roscpp rospy sensor_msgs std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(checkboard_camera_calibrator src/checkboard_camera_calibrator.cpp)
target_link_libraries(checkboard_camera_calibrator ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(checkboard_camera_undistorter src/checkboard_camera_undistorter.cpp)
target_link_libraries(checkboard_camera_undistorter ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})