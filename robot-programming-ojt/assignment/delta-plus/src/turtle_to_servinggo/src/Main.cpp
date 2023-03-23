#include "turtle_to_servinggo/Recorder.h"
#include "turtle_to_servinggo/Replayer.h"
#include "turtle_to_servinggo/Player.h"
#include "ros/ros.h"

int main(int argc, char **argv) {

  char choice;
  std::string filename;
  std::string recordingTime;

  std::cout << "Select the mode: " << std::endl;
  std::cin >> choice;
  std::cout << "Input the filename: " << std::endl;
  std::cin >> filename;
  std::cout << "Input the recording time: " << std::endl;
  std::cin >> recordingTime;

  if (choice == '1') {
    ros::init(argc, argv, "turtle_to_servinggo");

    double max_linear, max_angular;
    max_linear = 0;
    max_angular = 0;

    ros::NodeHandle nh_param("~");
    nh_param.getParam("max_linear", max_linear);
    nh_param.getParam("max_angular", max_angular);

    Recorder recorder(max_linear, max_angular, filename);

    double start = ros::Time::now().toSec();

    // ros::Rate loop_rate(20);
    while(ros::Time::now().toSec() < start + (std::stod(recordingTime))) {
      ros::spinOnce();
      // loop_rate.sleep();
    }

    std::cout << "Running Completed!" << std::endl;
    recorder.recordSaver();
  }
  else if (choice == '2') {
    ros::init(argc, argv, "turtle_to_servinggo");

    double max_linear, max_angular;
    max_linear = 0;
    max_angular = 0;

    ros::NodeHandle nh_param("~");
    nh_param.getParam("max_linear", max_linear);
    nh_param.getParam("max_angular", max_angular);

    Player player(max_linear, max_angular, filename);

    double start = ros::Time::now().toSec();

    // ros::Rate loop_rate(20);
    while(ros::Time::now().toSec() < start + (std::stod(recordingTime))) {
      ros::spinOnce();
      // loop_rate.sleep();
    }

    player.recordSaver();
  }
  else if (choice == '3') {
    ros::init(argc, argv, "turtle_to_servinggo");

    Replayer replayer(filename);
    replayer.replayByServinggo();
  }
  else {
    std::cout << "Check out your input value..." << std::endl;
  }

  return 0;
}