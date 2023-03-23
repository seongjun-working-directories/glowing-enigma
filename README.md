# 코가로보틱스 인턴 과정 기록용 레포지토리</small>

> ### 기본 정보

- 기업명 : 코가로보틱스(http://coga-robotics.com/)
- 소속팀명 : 자율주행 솔루션 사업본부
- 계약기간 : 2022.09.01 ~ 2023.02.28

<br/>

> ### <strong>'glowing-enigma'</strong> 레포지토리 정보
- robot-programming-ojt
  - C++/ROS/OpenCV4, 센서(RGB-D Camera, LiDAR, etc.), 장치(Wheel, Joystick, etc.) 등에 대한 활용 교육
  - robot-programming-ojt/assignment 내의 alpha, alpha-plus, beta, gamma, delta, delta-plus가 핵심 정보
- calibration-room
  - depth_(chessboard_)pose_estimation : RGB-D 카메라의 자세 및 위치 추정 프로그램으로 구성됨
  - lidar_pose_estimation : 2D LiDAR의 자세 및 위치 추정 프로그램으로 구성됨
  - camera_pose_estimation : RGB 카메라의 자세 및 위치 추정 프로그램으로 구성됨
  - pose_estimation_gui : 브라우저 상에서 코드를 핸들링할 수 있도록 구성됨
- other-robot-settings
  - Servinggo(분류: 서빙로봇), KT Smart Chair(분류: 자율주행 휠체어) 등에 대한 세팅 방법
- other-programs
  - camera-calibration : 카메라 내부 파라미터 계산 및 왜곡 보정 프로그램
  - ros_gui_viewer : Servinggo 등에서 보내는 Map 정보 및 주행 정보 시각화 프로그램
  - dual-band-wifi-router : AP 선정을 위한 아마존 크롤링 및 데이터 엑셀화 프로그램

<br/>

> ### 코가로보틱스 인턴 주간 보고서

- 업무요약
  - (국문) 코가로보틱스 자율주행 솔루션 개발
  - (영문) Coga-Robotics Autonomous Driving Solution Development
- 1주차
  - 코가로보틱스 회사 소개 및 코가로보틱스 팀별 직무 이해
    - 코가로보틱스 회사의 전반적인 구성에 대한 교육
- 2주차
  - 코가로보틱스 robotics engineering OJT 1주차
    - Ubuntu 20.04 환경에서 C++ 및 ROS를 활용한 기본적인 로봇 프로그래밍
- 3주차
  - 코가로보틱스 robotics engineering OJT 번외
    - C++, Opencv4, ROS를 활용한 영상 처리 프로그래밍
- 4주차
  - 코가로보틱스 robotics engineering OJT 2주차
    - LSLidar, Astra Camera 등의 센서와 URDF를 활용한 센서 정보 취득 및 rviz 시각화
- 5주차
  - 코가로보틱스 robotics engineering OJT 3주차
    - ROS 이기종 기기 간 유선/무선 통신
    - ROS 카메라 센서 영상 취득 및 배포 내용 처리
- 6주차
  - 코가로보틱스 robotics engineering OJT 4주차
    - 조이스틱 등을 활용한 서빙고(Servinggo) 스켈레톤 조작 등
    - 이동 기록대로 재주행 가능하도록 text file과 bag file에 기록 등
- 7주차
  - IFS 박람회(https://www.ifskorea.co.kr/ko-kr.html) 및 로보월드(http://www.robotworld.or.kr/wp/) 준비
    - 서빙고(Servinggo) 센서(astra_camera, depth2laser), URDF, AP(Network),  주행 특성(주행 속도, 회전 속도, 긴급 정지 거리 등) 설정
  - ROS GUI Viewer 개발(Electronjs, Vuejs, Nodejs)
    - ros2d / ros3d로 매트릭, 토폴로지 매핑된 결과물을 electron 앱으로 볼 수 있도록 개발
- 8주차
  - IFS 박람회(https://www.ifskorea.co.kr/ko-kr.html) 및 로보월드(http://www.robotworld.or.kr/wp/) 준비
    - 서빙고(Servinggo) 전시회 시나리오(공간 및 경로) 설계
  - Depth2Laser(https://github.com/mauriliodc/depth2laser의 자체 개발된 버전) 코드 분석
    - Depth2Laser 패키지는 카메라의 Depth 데이터를 해석하여, 장애물 여부를 구분하는 패키지
    - Depth2Laser 패키지의 로직 분석 및 이해
- 9주차
  - KT/대동/코가로보틱스 스마트체어 매핑
    - 동대문디자인프라자  DDP 디자인 페어 전시회 시연을 위한 스마트 체어 이동 시나리오 매핑
  - RGB Camera Intrinsic Calibration 구현
    - Chessboard를 활용한 카메라 내부 파라미터 계산
    - 카메라 내부 파라미터값을 이용해 카메라 왜곡을 보정
- 10주차
  - LiDAR 데이터 조작 구현
    - LiDAR의 LaserScan, PointCloud2, PCL 데이터 조작용 프로그램 구현
  - RGB Camera Extrinsic Calibration 구현
    - Chessboard를 활용한 카메라 외부 파라미터 계산
    - 카메라 외부 파라미터값을 이용해 카메라 자세를 출력
- 11주차
  - RGB-D Camera Pose Estimation 구현 중 (1/4)
    - 카메라 깊이 정보(Depth) 및 영상 정보(Color)를 토대로 카메라의 위치 및 자세를 계산하는 프로그램 구현
  - Ros Astra Camera(Stereo_s_u3), Stella 400, Intel Realsense 등 다양한 Depth Camera 활용
- 12주차
  - RGB-D Camera Pose Estimation 구현 중 (2/4)
    - 카메라 깊이 정보(Depth) 및 영상 정보(Color)를 토대로 카메라의 위치 및 자세를 계산하는 프로그램 구현
  - Redis 및 ROS를 함께 활용할 수 있는 방안에 대한 연구
    - 에러 데이터 보관, ROS Bag 파일 관리 등
- 13주차
  - RGB-D Camera Pose Estimation 구현 완료 (3/4)
    - 카메라 깊이 정보(Depth) 및 영상 정보(Color)를 토대로 카메라의 위치 및 자세 계산
    - Chessboard / Charucoboard / Arucoboard Version의 Color 이미지와 이에 대응되는 Depth Pointcloud 데이터를 기반으로 한 센서 자세 추정
    - yaml 파일을 통한 외부 값 주입, 실시간 계산 및 tf 반영 기능, rigid transformation 직접 구현 등
- 14주차
  - RGB-D Camera Pose Estimation 구현 검증 (4/4)
    - 실제 로봇에 탑재된 카메라의 위치 및 자세와 일치하는지 검증
    - 특정된 포인트와 실제 월드 좌표계의 포인트의 차이를 수치화한 후 로봇의 URDF에 적용
  - Servinggo Mini의 센서 오류 문제 해결
    - 사용 센서 : LS01B LiDAR, LSM10 LiDAR, Stereo_s_u3 Camera, SVPro RGB Camera, 터치형 디스플레이 등
    - 포트 연결 순서 및 전력 공급량 문제 발견
- 15주차
  - RGB Camera, RGB-D Camera, 2D LiDAR 통합 프로그램 개발 (1/3)
    - RGB Camera, RGB-D Camera, 2D LiDAR 센서의 위치 및 자세 추정 노드를 통합
    - Web Browser에서 UI로 관리가 가능하도록 개발
- 16주차
  - RGB Camera, RGB-D Camera, 2D LiDAR 통합 프로그램 개발 (2/3)
    - RGB Camera, RGB-D Camera, 2D LiDAR 센서의 위치 및 자세 추정 노드를 통합
    - Web Browser에서 UI로 관리가 가능하도록 개발
- 17주차
  - RGB Camera, RGB-D Camera, 2D LiDAR 통합 프로그램 개발 (3/3)
    - RGB Camera, RGB-D Camera, 2D LiDAR 센서의 위치 및 자세 추정 노드를 통합
    - Web Browser에서 UI로 관리가 가능하도록 개발