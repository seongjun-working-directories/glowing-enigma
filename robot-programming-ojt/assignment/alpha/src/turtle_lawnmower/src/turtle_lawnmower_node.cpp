#include <turtle_lawnmower/TurtleLawnmower.hpp>

double deg_to_radian(double angle_deg);

int main(int argc, char **argv) {
  ros::init(argc, argv, "turtle_lawnmower_node");

  // direction과 moving_lengths 간 정보 무결성이 훼손될 수 있기 때문에
  // 정교하게 디벨롭하려면, 따로 클래스 또는 구조체를 만들어주는게 필요
  int direction_logs[11] = {true, true, true, true, true, false, true, false, false, false, true};
  int moving_lengths[12] = {4, 90, 4, 135, 5, 90, 5, 45, 4, 90, 4, 0};
  int moving_lengths_counter = 0;

  int circle_logs_counter = 0;
  const int circle_logs_length = 4;
  bool circle_logs[circle_logs_length][3] = {
    {true, false, true},
    {true, true, false},
    {false, true, false},
    {false, true, true}
  };

  TurtleLawnmower TLMower;

  while(!TLMower.getStart()) {
    ros::spinOnce();
  }

  std_srvs::Empty empty;

  TLMower.callKill();
  TLMower.callSpawn();

  // 1차 주행
  while (moving_lengths[moving_lengths_counter] != 0) {
    if (moving_lengths_counter % 2 == 0) {
      TLMower.move(2, moving_lengths[moving_lengths_counter], direction_logs[moving_lengths_counter]);
    }
    else {
      TLMower.rotate(2, deg_to_radian(moving_lengths[moving_lengths_counter]), direction_logs[moving_lengths_counter]);
    }
    moving_lengths_counter++;
  }

  TLMower.callReset(empty);

  TLMower.callKill();
  TLMower.callSpawn();

  // 2차 주행
  while (circle_logs_counter < circle_logs_length) {
    TLMower.circle(
      circle_logs[circle_logs_counter][0],
      circle_logs[circle_logs_counter][1],
      circle_logs[circle_logs_counter][2]
    );
    circle_logs_counter++;
  }

  // 주행 완료 후 정렬
  TLMower.rotate(2, deg_to_radian(180), false);

  // ros::Rate loop_rate(10);
  // loop_rate.sleep();

  ros::spin();

  return 0;
}

double deg_to_radian(double angle_deg) {
  // double angle_radian = angle_deg/180 * 3.141592;
  double angle_radian = angle_deg/180 * 3.141592;
  return angle_radian;
}