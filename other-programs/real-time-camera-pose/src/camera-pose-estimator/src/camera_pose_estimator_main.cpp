#include "camera-pose-estimator/CameraPoseEstimator.hpp"

int main(int argc, char **argv)
{
  CameraPoseEstimator camera_pose_estimator = CameraPoseEstimator();

  while (true)
  {
    int error_code = 0;

    std::string commands;
    std::cout << "===== WHAT PROGRAM DO YO WANNA RUN ? (1 / 2 / 3 / 4) =====" << std::endl;
    std::cout << "(1) INTRINSIC" << std::endl;
    std::cout << "(2) EXTRINSIC(DEFAULT)" << std::endl;
    std::cout << "(3) EXTRINSIC(CUSTOMIZED)" << std::endl;
    std::cout << "(4) EXIT" << std::endl;
    std::cin >> commands;

    if (commands == "1")
    {
      camera_pose_estimator.calculate_intrinsic_parameters();
    }
    else if (commands == "2")
    {
      error_code = camera_pose_estimator.estimate_real_time_pose(false);
    }
    else if (commands == "3")
    {
      error_code = camera_pose_estimator.estimate_real_time_pose(true);
    }
    else if (commands == "4")
    {
      printf("BYE~\n\n");
      return 0;
    }
    else
    {
      // ERROR CODE '-3' >> NOT_VALID_INPUT_VALUE
      error_code = -3;
    }

    switch(error_code)
    {
    case 0:
      printf("The program has completed successfully!\n\n");
      break;
    case -1:
      printf("ERROR_CODE '-1' >> CAMERA_NOT_OPENED\n\n");
      break;
    case -2:
      printf("ERROR_CODE '-2' >> IMAGE_CANNOT_READ\n\n");
      break;
    case -3:
      printf("ERROR_CODE '-3' >> NOT_VALID_INPUT_VALUE\n\n");
    }
  }

  return 0;
}