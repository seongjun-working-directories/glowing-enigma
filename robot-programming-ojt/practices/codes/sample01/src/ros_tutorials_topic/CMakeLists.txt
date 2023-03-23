# 운영체제에 설치된 cmake의 최소 요구 버전
cmake_minimum_required(VERSION 3.0.2)
# 패키지명과 동일
project(ros_tutorials_topic)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

# catkin 빌드 시 요구되는 구성 요소 패키지
# 사용자가 만든 패키지가 의존하는 다른 패키지를 먼저 설치하게 만드는 옵션
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

# rospy를 사용할 때 설정하는 옵션
# 파이썬 설치 프로세스인 setup.py를 부르는 역할
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

# 메시지 파일을 추가하는 옵션
# FILES를 사용하면 현재 패키지 폴더의 msg 폴더 안의 .msg 파일들을 참조하여 헤더 파일(*.h)을 자동으로 생성
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )
add_message_files(
  FILES
  MsgTutorial.msg
)

# 서비스 파일을 추가하는 옵션
# FILES를 사용하면 현재 패키지 폴더의 srv 폴더 안의 .srv 파일들을 참조하여 헤더 파일(*.h)을 자동으로 생성
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

# 액션 파일을 추가하는 옵션
# FILES를 사용하면 현재 패키지 폴더의 action 폴더 안의 .action 파일들을 참조하여 헤더 파일(*.h)을 자동으로 생성
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

# 의존하는 메시지를 설정하는 옵션
# DEPENDENCIES 옵션에 의해 std_msgs 메시지 패키지를 사용하겠다는 설정
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )
generate_messages(
  DEPENDENCIES
  std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

# dynamic_reconfigure를 사용할 때 참조하는 설정 파일을 불러오는 설정
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################

# 캐킨 빌드 옵션
# INCLUDE_DIRS : 뒤에 설정해둔 패키지 내부 폴더인 include의 헤더파일을 사용하겠다는 설정
# LIBRARIES : 뒤에 설정한 패키지의 라이브러리를 사용하겠다는 설정
# CATKIN_DEPENDS : 뒤에 roscpp나 std_msgs 등 의존하는 패키지를 지정
# DEPENDS는 시스템 의존 패키지를 기술하는 설정
catkin_package(
#  INCLUDE_DIRS include
 LIBRARIES ros_tutorials_topic
 CATKIN_DEPENDS message_generation roscpp std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## 인클루드 폴더를 지정할 수 있는 옵션
## ${catkin_INCLUDE_DIRS} : 각 패키지 안의 include 폴더를 의미하며
## 해당 include 폴더들 안의 헤더파일을 이용하겠다는 설정
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## 빌드 후 생성할 라이브러리를 선언
## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/ros_tutorials_topic.cpp
# )

## 위의 라이브러리 및 실행 파일을 빌드하기 앞서 생성해야 하는 의존성이 있는 메시지 및 dynamic reconfigure가 있다면
## 우선적으로 이를 수행하라는 설정
## add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## 빌드 후 생성할 실행 파일에 대한 옵션을 지정
## src/topic_publisher.cpp 파일을 참조하여 topic_publisher 실행파일을 생성하라는 내용
## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
## 양식 : add_executable(${PROJECT_NAME}_node src/ros_tutorials_topic_node.cpp)
add_executable(topic_publisher src/topic_publisher.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

# 해당 라이브러리 및 실행 파일을 빌드하기에 앞서 생성해야 할 의존성이 있는 메시지 및 dynamic reconfigure가 있다면
# 우선적으로 이를 수행하라는 설정
# add_dependencies(topic_publisher ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(topic_publisher ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# 지정 실행 파일을 생성하기에 앞서 링크해야 하는 라이브러리와 실행 파일을 링크해주는 옵션
target_link_libraries(topic_publisher
  ${catkin_LIBRARIES}
)

add_executable(topic_subscriber src/topic_subscriber.cpp)
add_dependencies(topic_subscriber ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(topic_subscriber ${catkin_LIBRARIES})

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# catkin_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_ros_tutorials_topic.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
