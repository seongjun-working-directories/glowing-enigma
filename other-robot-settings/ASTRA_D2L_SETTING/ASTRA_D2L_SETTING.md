> ## ros_astra_camera 및 depth2laser 세팅

---
<br/>

> ### Solution T2(Servinggo Skeleton) 초기화 후 세팅(ros_astra_camera)

- \# astra, depth2laser는 sub(192.168.2.3) 에서 처리됨

- $ ssh cona@192.168.2.3 -XC

<br/>

- \# 다 켜져있는지 확인
- rosnode list
- rostopic echo /camera/depth/points
- roslaunch astra_camera stereo_u_s3.launch
- sudo apt install libgoogle-glog-dev
- roslaunch astra_camera stereo_u_s3.launch
- rostopic echo /camera/depth/points

<br />

- \# depth2laser를 담은 workspace를 catkin_make 하면 자동으로 CoNA_Navi_d2l에 넘어감
- \# git clone https://github.com/CogAplex/depth2laser.git # 코드 수정 시

<br/>

- $ git clone https://github.com/CogAplex/CoNA_Navi_d2l.git
- $ cd /home/cona/CoNA_Navi_d2l
- $ git checkout test2
- $ ls
- $ cd src
- $ ls
- $ cd astra_camera/
- $ ls
- $ cd scripts/
- $ ./create_udev_rules

<br/>

- $ cd ..
- $ ls
- $ cd params/
- $ ls
- $ sudo vim camera_params_template.yaml
- $ cd ..
- $ cd launch
- $ sudo vim stereo_s_u3.launch
- $ cd ~/CoNA_Navi_d2l/
- $ catkin_make install
- $ source devel/setup.bash
- $ sudo apt install libgoogle-glog-dev

<br/>

- \# ssh cona@192.168.2.2 -XC 에서 roscore를 켜줘야 함
- $ roslaunch astra_camera stereo_s_u3.launch
- \# 다른 창에서 ssh cona@192.168.2.3 -XC 에서 rostopic echo /camera/depth/points 확인

<br/>

- \# 두 대 세팅
- $ udevadm info /dev/video0
- $ udevadm info /dev/video2
- \# cd ~/CoNA_Navi_d2l/src/astra_camera/launch
- $ sudo vim multi_astra.launch
- $ catkin_make install

<br/>

- \# ssh cona@192.168.2.2 -XC에서
- $ roslaunch cona cona_all_m10.launch

<br/><br/>

> ### ROS Astra Camera 기타 정보 1

- $ ssh cona@192.168.2.3 -XC
- $ rostopic list
- $ rostopic echo /camera/depth/points    # 결과값이 나오지 않음
- $ roslaunch astra_camera stereo_s_u3.launch

<br/>

... logging to /home/cona/.ros/log/1352f5f8-4aae-11ed-9c1e-ff72a3743a4d/roslaunch-cona-2896.log<br/>
Checking log directory for disk usage. This may take a while.<br/>
Press Ctrl-C to interrupt<br/>
Done checking log file disk usage. Usage is <1GB.<br/>

started roslaunch server http://192.168.2.3:42935/<br/>

SUMMARY<br/>
\========

PARAMETERS
 * /camera/camera/camera_name: camera
 * /camera/camera/color_depth_synchronization: False
 * /camera/camera/color_fps: 30
 * /camera/camera/color_height: 400
 * /camera/camera/color_roi_height: -1
 * /camera/camera/color_roi_width: -1
 * /camera/camera/color_roi_x: -1
 * /camera/camera/color_roi_y: -1
 * /camera/camera/color_width: 640
 * /camera/camera/depth_align: True
 * /camera/camera/depth_fps: 30
 * /camera/camera/depth_height: 400
 * /camera/camera/depth_roi_height: -1
 * /camera/camera/depth_roi_width: -1
 * /camera/camera/depth_roi_x: -1
 * /camera/camera/depth_roi_y: -1
 * /camera/camera/depth_scale: 1
 * /camera/camera/depth_width: 640
 * /camera/camera/enable_color: False
 * /camera/camera/enable_d2c_filter: False
 * /camera/camera/enable_depth: True
 * /camera/camera/enable_ir: False
 * /camera/camera/enable_pointcloud: True
 * /camera/camera/enable_pointcloud_xyzrgb: False
 * /camera/camera/enable_reconfigure: False
 * /camera/camera/ir_fps: 30
 * /camera/camera/ir_height: 400
 * /camera/camera/ir_width: 640
 * /camera/camera/keep_alive: False
 * /camera/camera/keep_alive_interval: 15
 * /camera/camera/number_of_devices: 1
 * /camera/camera/oni_log_level: verbose
 * /camera/camera/oni_log_to_console: False
 * /camera/camera/oni_log_to_file: False
 * /camera/camera/publish_tf: True
 * /camera/camera/reconnection_delay: 3
 * /camera/camera/serial_number: 
 * /camera/camera/tf_publish_rate: 10.0
 * /camera/camera/use_uvc_camera: False
 * /camera/camera/uvc_camera_format: mjpeg
 * /camera/camera/uvc_flip: False
 * /camera/camera/uvc_product_id: 0x0511
 * /camera/camera/uvc_retry_count: 100
 * /camera/camera/uvc_vendor_id: 11205
 * /rosdistro: noetic
 * /rosversion: 1.15.13

NODES
  /camera/
    camera (astra_camera/astra_camera_node)

ROS_MASTER_URI=http://192.168.2.2:11311/

process[camera/camera-1]: started with pid [2904]<br/>
/home/cona/CoNA_Navi_d2l/install/lib/astra_camera/astra_camera_node: error while loading shared libraries: libglog.so.0: cannot open shared object file: No such file or directory<br/>
[camera/camera-1] process has died [pid 2904, exit code 127, cmd /home/cona/CoNA_Navi_d2l/install/lib/astra_camera/astra_camera_node /camera/depth/color/points:=/camera/depth_registered/points __name:=camera __log:=/home/cona/.ros/log/1352f5f8-4aae-11ed-9c1e-ff72a3743a4d/camera-camera-1.log].<br/>
log file: /home/cona/.ros/log/1352f5f8-4aae-11ed-9c1e-ff72a3743a4d/camera-camera-1*.log<br/>
all processes on machine have died, roslaunch will exit<br/>
shutting down processing monitor...<br/>
... shutting down processing monitor complete<br/>
done<br/><br/>

- $ sudo apt install libgoogle-glog-dev
- $ roslaunch astra_camera stereo_s_u3.launch
- \# Servinggo 기기를 재부팅한 뒤
- $ ssh cona@192.168.2.3 -XC
- $ rostopic echo /camera/depth/points
- \# 이제부터 결과값이 들어오는 것을 알 수 있음

<br/><br/>

> ### ROS Astra Camera 기타 정보

- [TIPS] 잘 안되는 거 같을 때 / 리부팅 후 astra_camera가 안 켜졌을 때 확인하는 방법
  - cona@cona에서 ls -> vim start.sh
  - gnome-session-properties 입력하면, depth2laser가 뜨는데,
  - /home/cona/start.sh를 실행시키는 것
  - 체크 표시를 풀면 자동으로 켜지지 않음(depth2laser와 astra_camera가 자동으로 켜지지 않음)

<br/>

- astra 및 depth 세팅
  - \# git clone orbbec ros_astra_camera
  - [56-orbbec-usb.rules]
  - ./scripts/create_udev_rules
  - cd params/
  - vim camera_params_template.yaml
  - depth만 사용
    - '''
    - enable_pointcloud_xyzrgb : false # 3 line
    - color_height : 400
    - depth_height : 400
    - ir_height : 400
    - enable_color: false
    - enable_ir: false
    - '''
  - vim stereo_s_u3.launch
  - param -> use_uvc_camera value : false
  - ros_astra_camera 깃허브에 가서 디펜던시 설치

<br/>

- [`대동`] astra 두 대 이상
  - $ vim multi_astra.launch
    - \<launch>
      - \<!-- unique camera name-->
      - \<arg name="camera_name" default="camera"/>
      - \<!-- Hardware depth registration -->
      - \<arg name="3d_sensor" default="stereo_s_u3"/>
      - \<!-- stereo_s_u3, astrapro, astra -->
      - \<arg name="camera1_prefix" default="01"/>
      - \<arg name="camera2_prefix" default="02"/>
      - \<arg name="camera1_serila_number" default="AY2T1120301"/>
      - \<arg name="camera2_serila_number" default="AY2T1120208"/>
      <br/><br/>
      - \<include file="$(find astra_camera)/launch/$(arg 3d_sensor).launch">
      - \<arg name="camera_name" value="$(arg camera_name)_$(arg camera1_prefix)"/>
      - \<arg name="serial_number" value="$(arg camera1_serila_number)"/>
      - \</include>
      <br/><br/>
      - \<include file="$(find astra_camera)/launch/$(arg 3d_sensor).launch">
      - \<arg name="camera_name" value="$(arg camera_name)_$(arg camera2_prefix)"/>
      - \<arg name="serial_number" value="$(arg camera2_serila_number)"/>
      - \</include>
      - \<node pkg="tf2_ros" type="static_transform_publisher" name="camera_tf" args="0 0 0 0 0 0 camera_01_link camera_02_link"/>
    - \</launch>
  - 위의 launch 파일과 같이 변경할 것

- cona@cona:~/CoNA_Navi_d2l/install/share/astra_camera$ 에서 바꾸든
- src에서 바꾸고 catkin_make install을 하든 둘 중 하나는 해야 됨

<br/>

- \<arg name="3d_sensor" default="stereo_s_u3" />
- \<arg name="camera1_serial_number" default=
- \<arg name="camera2_serial_number" default=
- udevadm info /dev/video0 의 ID_SERIAL_SHORT 를 복사해서 붙여 넣음
- udevadm info /dev/video1 도 같은 카메라
- udevadm info /dev/video2 가 다른 카메라
- usb_cam -> 하나당 카메라 4개 잡힘

<br/>

- // AY가 아스트라
- AY2T1120301, AY2T1120208

<br/>

- \<node pkg="tf2_ros" type="static_transform_publisher" name="camera_tf" args="0 0 0 0 0 0 camera01_link camera02_link"/> 와 같이 되어 있다면,
- \<node pkg="tf2_ros" type="static_transform_publisher" name="camera_tf" args="0 0 0 0 0 0 camera_01_link camera_02_link"/> 로 변경해야 함
