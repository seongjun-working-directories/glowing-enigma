#include "turtle_to_servinggo/Replayer.h"

#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

Replayer::Replayer(std::string filename) {
  this->filename = filename;

  std::ifstream file(this->filename);
  std::string tmp;

  if (file.is_open()) {
    while (file.good()) {
      getline(file, tmp);
      this->record.push_back(tmp);
    }
  }

  pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
};

Replayer::~Replayer() {

};

void Replayer::replayByServinggo() {
  geometry_msgs::Twist read_cmd_vel;
  std::string dummy;

  std::cout << "START By Entering '1' value>>>" << std::endl;
  std::cin >> dummy;

  std::vector<std::string>::iterator it = std::begin(this->record);

  ros::Rate loop_rate(20);

  while (it != std::end(this->record)) {

    // [TEST] std::cout << *it << "\n";
    std::string line_info = (*it);

    std::vector<std::string> x_and_z = Replayer::splitInfo(line_info);

    std::string x_val = x_and_z.front();
    std::string z_val = x_and_z.back();

    /* [TEST]
    std::cout << "x_val :: " << x_val << std::endl;
    std::cout << "z_val :: " << z_val << std::endl;
    */

    if (x_val == " " || x_val == "" || z_val == " " || z_val == "") {
      read_cmd_vel.linear.x = 0;
      read_cmd_vel.angular.z = 0;
    }
    else {
      read_cmd_vel.linear.x = std::stod(x_val);
      read_cmd_vel.angular.z = std::stod(z_val);
    }


    /* [TEST]
    if (read_cmd_vel.linear.x != 0) {
      read_cmd_vel.linear.x = read_cmd_vel.linear.x > 0 ? 1 : -1;
      read_cmd_vel.linear.x *= 0.1;
    }
    if (read_cmd_vel.angular.z != 0) {
      read_cmd_vel.angular.z = read_cmd_vel.angular.z > 0 ? 1 : -1;
      read_cmd_vel.angular.z *= 0.1;
    }
    */

    /* [TEST]
    */
    std::cout << "(" << read_cmd_vel.linear.x << ", ";
    std::cout << read_cmd_vel.angular.z << ")" << std::endl;

    this->pub.publish(read_cmd_vel);

    ros::spinOnce();

    loop_rate.sleep();

    ++it;
  }
}

// "0.200000 0.000332" -> ["0.200000", "0.000332"]
std::vector<std::string> Replayer::splitInfo(std::string &line_info) {
  std::vector<std::string> x_and_z;
  boost::split(x_and_z, line_info, boost::is_any_of(" "));
  return x_and_z;
}