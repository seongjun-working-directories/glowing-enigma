> ## ROS GUI 켜기

---
<br>


> ### NAS Link : nas.coga-robotics.com

<br/>

> ### files > 연구소 > 1_자율주행 솔루션 사업본부 > 2_프로젝트

<br/>

> ### ROS GUI 켜기

- sudo apt-get update
- sudo apt-get install nodejs
- sudo apt-get install npm

<br/>

- sudo npm cache clean -f
- sudo npm isntall -g n
- sudo n stable
- node -v
- sudo npm install -g npm
- npm -v

<br/>

- npm install
- sudo apt install curl
- curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
- echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
- sudo apt-get update && sudo apt-get install yarn

<br/>

- sudo apt-get install ros-noetic-rosbridge-suite
- (만약 로봇에 켜져 있지 않은 경우에만) roslaunch rosbridge_server rosbridge_websocket.launch
- npm start