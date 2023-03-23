# turtlesim_tutorials

---
<br/>

> ## COGA-ROBOTICS OJT 1st Task - NOTICE AREA

- Contents
  - 3년째 탈락의 고배를 마시고있는 거북이들의 극악 난이도 운전면허 기능시험을 당신의 알고리즘으로 통과시켜 주세요!
  - 면허 시험장 및 응시자
    - rosrun turtlesim turtlesim_node ( 명령어 아님 : turtlesim_node)
    - 응시자: 2마리
      - rosservice 중 새로운 거북이 탄생시키는 API있음 -> /spawn
  - 코스 및 점수
    - 총 2개 코스
    - 각 코스의 구간마다 부분 점수가 존재함 (1 st : 20/20/20/20/20, 2 nd : 30/30/40)
    - 점수 : 총 200점 만점 ( 응시자 2 마리 점수를 합산 )

<br/>

- Conditions
  - 구현 언어: C/C++ or Python
  - 아래 코스를 모두 통과하는 ROS 노드를 구현하여 세미나 때 라이브 데모
  - 각 구간를 주행하는 거북이들의 모션(cmd_vel)은 모두 구현할 것
    - 단, 각 구간마다 ros topic으로 명령을 주는 것은 상관없음
  - 알고리즘 트리거 메시지
    - 메시지 타입: std_msgs/Bool.msg
    - 메시지 토픽: /driver_start
    - 메시지 내용: ’true’

<br/>

- Result Examples
![Result_Example_1](./assets/Result_Example_1.png)
![Result_Example_2](./assets/Result_Example_2.png)
![Result_Example_3](./assets/Result_Example_3.png)

<br/><br/>

> ## COGA-ROBOTICS OJT 1st Task - SOLUTION AREA

- Structure of `turtlesim_tutorials/src`
  - turtle_lawnmower
    - ./include/turtle_lawnmower/TurtleLawnmower.hpp
    - ./src/turtle_lawnmower_function.cpp
    - ./src/turtle_lawnmower_node.cpp
    - ./CMakeLists.txt
    - ./package.xml

<br/>

- Description of each files
  - ./include/turtle_lawnmower/TurtleLawnmower.hpp
    - TurtleLawnmower 클래스의 선언부
  - ./src/turtle_lawnmower_function.cpp
    - TurtleLawnmower 클래스의 구현부
  - ./src/turtle_lawnmower_node.cpp
    - main 함수

<br/>

- Core of `turtlesim_tutorials/src` : TurtleLawnmower 클래스

  - Variables [ Access Modifier = default ]
    - ros::NodeHandle nh
    - ros::Subscriber sub
    - ros::Publisher pub
    - ros::Subscriber msg_sub
    - ros::Publisher msg_pub
    - turtlesim::Pose turtlesim_pose
    - ros::ServiceClient reset
    - ros::ServiceClient spawn
    - ros::ServiceClient kill_spawn
    - turtlesim::Spawn srv
    - turtlesim::Kill kill
    - bool start<br/><br/>

  - Methods [ Access Modifier = public ]
    - TurtleLawnmower();
    - ~TurtleLawnmower();
    - void turtleCallback(const turtlesim::Pose::ConstPtr& msg);
    - void boolCallback(const std_msgs::BoolConstPtr &msg);
    - void move(double speed, double distance, bool isForward);
    - void rotate(double angular_speed, double angle_radian, bool direction);
    - void circle(bool bigSize, bool halfCircle, bool direction);
    - void callReset(std_srvs::Empty empty);
    - void callSpawn();
    - void callKill();
    - bool getStart();