#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

class Replayer {
private:
  ros::NodeHandle nh;
  ros::Publisher pub;

  std::string filename;
  std::vector<std::string> record;

public:
  Replayer(std::string filename);
  ~Replayer();
  void replayByServinggo();
  std::vector<std::string> splitInfo(std::string &line_info);
};