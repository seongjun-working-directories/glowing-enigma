cmake_minimum_required(VERSION 3.0.2)
project(ros_tutorials_service)

# 캐킨 빌드할 때 요구되는 구성요소 패키지
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)

# 서비스 선언 : SrvTutorial.srv
add_service_files(
  FILES
  SrvTutorial.srv
)

# 의존하는 메시지를 설정하는 옵션
generate_messages(
  DEPENDENCIES
  std_msgs
)

# 캐킨 패키지 옵션 : 라이브러리, 캐킨 빌드 의존성, 시스템 의존 패키지 기술
catkin_package(
 LIBRARIES ros_tutorials_service
 CATKIN_DEPENDS message_generation roscpp std_msgs
)

# 인클루드 디렉토리 설정
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# service_server 노드에 대한 빌드 옵션
add_executable(service_server src/service_server.cpp)
add_dependencies(
  service_server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS0}
)
target_link_libraries(service_server ${catkin_LIBRARIES})

# service_client 노드에 대한 빌드 옵션
add_executable(service_client src/service_client.cpp)
add_dependencies(
  service_client ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(service_client ${catkin_LIBRARIES})
