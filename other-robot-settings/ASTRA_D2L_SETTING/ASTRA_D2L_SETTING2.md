> ## ros_astra_camera 및 depth2laser 세팅

---
<br/>

> ### Solution T2(Servinggo Skeleton) 초기화 후 세팅(depth2laser)

- \# $ git clone depth2laser -> CogAplex/CoNA_Navi_d2l -> git checkout test_2
- \# $ git clone CogAplex/depth2laser -> git checkout sc_2

<br/>

- 카메라가 1개인 경우(in ~/CoNA_Navi_d2l/install/share/depth2laser/launch)
  - $ vim depth2laser.launch
    - run_vis : visualize 현재 어떤 바닥에 피팅, 디버깅용 툴 (보통 false, 에러 잡을 때 true)
    - new_d2l : 새로 만든 depth2laser를 쓸 건지 여부 -    - 보통 true
    - use_samp_gap : true x, y 샘플링 / false y 샘플링
    - cam_name : 토픽 sub/pub 이름 지정
    <br/><br/>
    - use_samp_gap이 false면 sampling_gap 파라미터를 사용
    - use_samp_gap이 true면 h_sampling_gap, w_sampling_gap 파라미터 사용
    - side_gap : 끝단에 노이즈가 생기면 잘라내는 역할     - 사이드에 몇개의 포인트를 제거할 것인가를 결정
    - depth_range_min : z 높이를 몇미터까지 볼 것인가
    - vis_inlier_ref : 장애물 검출 시 평면으로부터 높이가 얼마를 넘는 것을 장애물로 시각화할 것인가 value = "0.05"     -5cm 보다 높으면 장애물
    - inlier_ref : 평면 뽑아내는 기준이 되는 포인트
    - ratio_ref : (코드 보며 설명)
    - valid_dist : 카메라 전방 x 축 최대 거리

<br/>

- 카메라가 2개인 경우
  - $ vim depth2laser_multi.launch
    - cam_name : camera_01, camera_02
    - scan_topic : scan3, scan4
    - pub_Scan_topic_frame : laser3, laser4

<br/>

- cf. 대동 multi 작업 시 중요한 것은 calibration, filtering, sampling, noise

<br/>

> ### CogAplex Brain Setting

- $ cd CoGA_CONAv2
- $ ls
- $ cd src
- $ ls
- $ cd ..
- $ catkin_make --pkg usb_cam
- $ ls

<br/>

- \# 대동 켜는 법
- $ roslaunch cona smartchair.launch
- $ roslaunch usb_cam usb_cam-test.launch
- $ roslaunch astra_camera multi_astra.launch
- $ roslaunch lsm10_v2 multi_m10.launch
- $ rviz

<br/>

- \# config.json 변경
- ~/data/CoNA에서 다음의 명령어 실행
- $ sudo vim config.json

'''<br/>
주행 관련된 것은 config.json 에서 변경함<br/>
LocalPathPlanner -> Rotation_gain_value: 10.0 -> 20.0 등<br/>
'''<br/>

<br/>

> ### CONAv2/src/Navigation/BungP_v2/src/cona/urdf

<br/>
'''<br/>
\<link name = "laser3, laser4" --> joint><br/>

\<joint name="robot2laser3" type="fixed">
  \<parent link="base_link" />
  \<child link="laser3" />
  \<origin xyz="0 0 0" rpy"0 0 0" />
\</joint>

\<joint name="robot2laser4" type="fixed">
  \<parent link="base_link" />
  \<child link="laser4" />
  \<origin xyz="0 0 0" rpy"0 0 0" />
\</joint>

<br/>

$ roslaunch cona smartchiar.launch <br/>
'''

<br/><br/>

> ### depth2laser 세팅

- \# astra 완료, depth2laser 시작
- $ ssh cona@192.168.2.3 -XC
- $ rosnode list # depth2laser 있으면 켜져 있는 것

<br/>

'''<br/>
cona@cona:~$ rosnode list<br/>
/amcl<br/>
/camera/camera<br/>
/cona<br/>
/depth2laser<br/>
/front_laser_m10<br/>
/guiServer<br/>
/laserscan_multi_merger<br/>
/map_modify<br/>
/node<br/>
/rear_laser<br/>
/robot_state_publisher<br/>
/rosapi<br/>
/rosbridge_websocket<br/>
/rosout<br/>
/servinggo<br/>
/sound_player<br/>
/task_manager<br/>
/tf2_web_republisher<br/>
'''<br/>

- $ rostopic echo /camera/inlier/points # 토픽 올라오면 켜진 것
- \# 카메라가 가려져 있으면 값이 올라오지 않음
- $ ssh cona@192.168.2.2 -XC
- $ cd data/CoNA
- $ vim config.json

<br/>

'''<br/>
"A_num_of_IMU": 0,
    "A_num_of_Joy": 1,
    "A_num_of_Laser": 3,
    "A_num_of_Odom": 1,
    "A_num_of_RGB": 1,
    "A_num_of_RGBD": 0,
    "Joy1_data_change_check": 0,
    "Joy1_imshow": 0,
    "Joy1_port": "/joy",
    "Joy1_source": "direct",
    "Joy1_use_pub_time": 0,
    "Laser1_data_change_check": 0,
    "Laser1_frame": "/laser1",
    "Laser1_frame_parent": "/base_link",
    "Laser1_imshow": 1,
    "Laser1_port": "/scan1",
    "Laser1_purpose": 5120,
    "Laser1_source": "ros",
    "Laser1_use_filter": 1,
    "Laser1_use_pub_time": 1,
    "Laser2_data_change_check": 0,
    "Laser2_frame": "/laser2",
    "Laser2_frame_parent": "/base_link",
    "Laser2_imshow": 1,
    "Laser2_port": "/scan2",
    "Laser2_purpose": 5120,
    "Laser2_source": "ros",
    "Laser2_use_filter": 1,
    "Laser2_use_pub_time": 1,
    "Laser3_data_change_check": 0,
    "Laser3_frame": "/laser3",
    "Laser3_frame_parent": "/base_link",
    "Laser3_imshow": 1,
    "Laser3_port": "/scan3",
    "Laser3_purpose": 5120,
    "Laser3_source": "ros",
    "Laser3_use_filter": 1,
    "Laser3_use_pub_time": 1,
'''

- \# Sensor: A_Number_Of_Laser -> 3이어야 depth2laser가 켜져 있는 것