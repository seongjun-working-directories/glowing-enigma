cmake_minimum_required(VERSION 3.0.2)
project(depth_camera_extrinsic_calibrator)
add_compile_options(-std=c++14)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  geometry_msgs
  image_transport
  laser_geometry
  pcl_conversions
  pcl_ros
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

find_package(OpenCV REQUIRED)
find_package(PCL 1.10 REQUIRED)
find_package(Boost REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
 LIBRARIES depth_camera_extrinsic_calibrator
 CATKIN_DEPENDS cv_bridge geometry_msgs image_transport laser_geometry pcl_conversions pcl_ros roscpp rospy sensor_msgs std_msgs
#  DEPENDS system_lib
)

include_directories(
include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(dce_calibrator src/dce_calibrator.cpp)
target_link_libraries(dce_calibrator ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(dce_initial_calibrator src/dce_initial_calibrator.cpp)
target_link_libraries(dce_initial_calibrator ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(camera_calibrator src/CameraCalibratorMain.cpp src/CameraCalibrator.cpp)
target_link_libraries(camera_calibrator ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})