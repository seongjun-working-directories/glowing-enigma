#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"
#include "joycontroller/DevJoyController.h"

DevJoyController::DevJoyController(double max_linear,double max_angular) {
    std::cout << "=== DevJoyController를 시작합니다. ===" << std::endl;

    sub = nh.subscribe("/joy", 1, &DevJoyController::joyCallback, this);
    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    if (max_linear > 0.5 || max_angular > 0.5) {
        this->max_linear = 0.33;
        this->max_angular = 0.33;
        return;
    }
    this->max_linear = max_linear;
    this->max_angular = max_angular;

    this->cmd_vel_bag.open("cmd_vel_tester.bag", rosbag::bagmode::Write);
}

DevJoyController::~DevJoyController() {
    std::cout << "=== DevJoyController를 종료합니다. ===" << std::endl;

    this->cmd_vel_bag.close();
};

void DevJoyController::joyCallback(const sensor_msgs::Joy &msg) {
    geometry_msgs::Twist toCmdVel;

    // std::cout << "max_linear>>>" << max_linear << std::endl;
    // std::cout << "max_angular>>>" << max_angular << std::endl;

    // NORTH(0, 1) OR SOUTH(0, -1) -> this value is for "linear x"
    // Max : Min = 1 : -1
    // Half the range of max, min values
    DevJoyController::setLinear(msg.axes[1] * DevJoyController::getMaxLinear());

    // EAST(-1, 0) OR WEST(1, 0) -> this value is for "angular z"
    // Max : Min = 1 : -1
    // Half the range of max, min values
    DevJoyController::setAngular(msg.axes[0] * DevJoyController::getMaxAngular());

    toCmdVel.linear.x = DevJoyController::getLinear();
    toCmdVel.angular.z = DevJoyController::getAngular();

    pub.publish(toCmdVel);
    cmd_vel_bag.write("/cmd_vel", ros::Time::now(), toCmdVel);
};
