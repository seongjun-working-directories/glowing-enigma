cmake_minimum_required(VERSION 3.0.2)
project(turtle_lawnmower)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  turtlesim
)

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
#   geometry_msgs
# )

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES turtle_lawnmower
 CATKIN_DEPENDS geometry_msgs roscpp turtlesim
#  DEPENDS system_lib
)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

add_executable(turtle_lawnmower_node src/turtle_lawnmower_node.cpp src/turtle_lawnmower_function.cpp)
add_dependencies(turtle_lawnmower_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(turtle_lawnmower_node ${catkin_LIBRARIES})