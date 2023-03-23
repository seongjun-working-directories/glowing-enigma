#include "camera-pose-estimator/CameraPoseEstimator.hpp"

// *** 생성자 및 소멸자 영역 *** //

CameraPoseEstimator::CameraPoseEstimator()
{
  double _intrinsic_parameters[] = {453.7543640, 0.0, 323.54632568, 0.0, 453.7543640, 241.9261780, 0.0, 0.0, 1.0};
  intrinsic_parameters = _intrinsic_parameters;
  double _radial_tangential_distortion[] = {0.0, 0.0, 0.0, 0.0};
  radial_tangential_distortion = _radial_tangential_distortion;

  camera_matrix = cv::Mat(3, 3, CV_64FC1, intrinsic_parameters);
  distortion_coefficients = cv::Mat(4, 1, CV_64FC1, radial_tangential_distortion);

  checkboard_dimensions = cv::Size(8-1, 11-1);
  square_size = 0.02410f;  // 단위: meter

  frames_per_second = 20;
}

CameraPoseEstimator::~CameraPoseEstimator() { }

// *** 카메라 외부 파라미터 영역 *** //

int CameraPoseEstimator::estimate_real_time_pose(bool change_intrinsic_parameters)
{
  if (change_intrinsic_parameters)
  {
    std::ifstream file;
    file.open("intrinsic+parameters.txt");

    std::string line;
    int line_count = 0;
    std::vector<double> _intrinsic_parameters;
    std::vector<double> _radial_tangential_distortion;
    while(std::getline(file, line))
    {
      if (line_count < 5) _intrinsic_parameters.push_back(std::stod(line));
      else _radial_tangential_distortion.push_back(std::stod(line));
    }
    intrinsic_parameters = &_intrinsic_parameters[0];
    radial_tangential_distortion = &_radial_tangential_distortion[0];

    camera_matrix = cv::Mat(3, 3, CV_64FC1, intrinsic_parameters);
    distortion_coefficients = cv::Mat(4, 1, CV_64FC1, radial_tangential_distortion);
  }

  cv::VideoCapture vid(0);

  // ERROR CODE '-1' >> CAMERA_NOT_OPENED
  if (!vid.isOpened()) return -1;

  while (true)
  {
    cv::waitKey(1000/frames_per_second);

    cv::Mat frame, draw_to_frame, axis_to_frame;
    cv::Mat rotation_vector, translation_vector;

    // ERROR CODE '-2' >> IMAGE_CANNOT_READ
    if (!vid.read(frame)) return -2;

    frame.copyTo(draw_to_frame);

    std::vector<cv::Point3f> object_points;
    std::vector<cv::Point2f> found_corners, image_points;
    std::vector<cv::Point3f> axis;

    create_world_coordinate_system(object_points);
    create_axis_points(axis);

    bool found = false;

    found = process_estimation_logic(frame, draw_to_frame, found_corners, object_points, rotation_vector, translation_vector);

    /* [TEST]
    cv::Mat _image1 = rotation_vector;
    for(int y = 0; y < _image1.rows; ++y)
    {
      cv::Vec3f* _image1_row = _image1.ptr<cv::Vec3f>(y);
      for(int x = 0; x< _image1.cols; ++x)
      {
        std::cout << _image1_row[x] << " ";
      }
      std::cout << std::endl;
    }
    cv::Mat _image2 = translation_vector;
    for(int y = 0; y < _image2.rows; ++y)
    {
      cv::Vec3f* _image2_row = _image2.ptr<cv::Vec3f>(y);
      for(int x = 0; x< _image2.cols; ++x)
      {
        std::cout << _image2_row[x] << " ";
      }
      std::cout << std::endl;
    }
    */

    if (found) {
      std::cout << "FOUND!" << std::endl;
      cv::projectPoints(axis, rotation_vector, translation_vector, camera_matrix, distortion_coefficients, image_points, cv::noArray(), 0);
      draw_axis(draw_to_frame, axis_to_frame, image_points, found_corners);


      std::cout << "rotation " << std::endl;
      std::cout << "x: " << rotation_vector.at<float>(0, 0) * (180 / M_PI) << " degree"
                << std::endl
                << "y: " << rotation_vector.at<float>(1, 0) * (180 / M_PI) << " degree"
                << std::endl
                << "z: " << rotation_vector.at<float>(2, 0) * (180 / M_PI) << " degree"
                << std::endl;
      std::cout << "translation " << std::endl;
      std::cout << "x: " << translation_vector.at<float>(0, 0) << " meter" << std::endl
                << "y: " << translation_vector.at<float>(1, 0) << " meter" << std::endl
                << "z: " << translation_vector.at<float>(2, 0) << " meter" << std::endl;
      cv::imshow("EXTRINSIC - CHESSBOARD FOUND", axis_to_frame);
    }
    else {
      std::cout << "NOT FOUND!" << std::endl;
      cv::imshow("EXTRINSIC - CHESSBOARD NOT FOUND", frame);
    }

  }

  return 0;
}

bool CameraPoseEstimator::process_estimation_logic(cv::Mat &frame, cv::Mat &draw_to_frame, std::vector<cv::Point2f> &found_corners, std::vector<cv::Point3f> &object_points, cv::Mat &rotation_vector, cv::Mat &translation_vector)
{
  bool found = false;
  cv::cvtColor(frame, draw_to_frame, cv::COLOR_BGR2GRAY);

  found = cv::findChessboardCorners(draw_to_frame, checkboard_dimensions, found_corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK);
  if (!found) return found;

  cv::TermCriteria term_criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001);
  cv::cornerSubPix(draw_to_frame, found_corners, cv::Size(11, 11), cv::Size(-1, -1), term_criteria);
  cv::solvePnPRansac(object_points, found_corners, camera_matrix, distortion_coefficients, rotation_vector, translation_vector, false, 100, 8.0, 0.99, cv::noArray(), cv::SOLVEPNP_EPNP);

  cv::Mat _image1 = rotation_vector;
  for(int y = 0; y < _image1.rows; ++y)
  {
    cv::Vec3f* _image1_row = _image1.ptr<cv::Vec3f>(y);
    for(int x = 0; x< _image1.cols; ++x)
    {
      std::cout << _image1_row[x] << " ";
    }
    std::cout << std::endl;
  }

  cv::Mat _image2 = translation_vector;
  for(int y = 0; y < _image2.rows; ++y)
  {
    cv::Vec3f* _image2_row = _image2.ptr<cv::Vec3f>(y);
    for(int x = 0; x< _image2.cols; ++x)
    {
      std::cout << _image2_row[x] << " ";
    }
    std::cout << std::endl;
  }

  return found;
}

void CameraPoseEstimator::draw_axis(cv::Mat &raw_image, cv::Mat &processed_image, std::vector<cv::Point2f> &image_points, std::vector<cv::Point2f> &corners)
{
  raw_image.copyTo(processed_image);

  cv::arrowedLine(processed_image, corners[0], image_points[0], cv::Scalar(0, 0, 255), 2, cv::LINE_AA, 0);
  cv::arrowedLine(processed_image, corners[0], image_points[1], cv::Scalar(0, 255, 0), 2, cv::LINE_AA, 0);
  cv::arrowedLine(processed_image, corners[0], image_points[2], cv::Scalar(255, 0, 0), 2, cv::LINE_AA, 0);
}

void CameraPoseEstimator::create_axis_points(std::vector<cv::Point3f> &axis)
{
  axis.push_back(cv::Point3f(3*square_size, 0.0f, 0.0f));
  axis.push_back(cv::Point3f(0.0f, 3*square_size, 0.0f));
  axis.push_back(cv::Point3f(0.0f, 0.0f, -3*square_size));
}

void CameraPoseEstimator::create_world_coordinate_system(std::vector<cv::Point3f> &object_points)
{
  for (int i=0; i<checkboard_dimensions.height; i++)
  {
    for (int j=0; j<checkboard_dimensions.width; j++)
    {
      object_points.push_back(cv::Point3f(i*square_size, j*square_size, 0));
    }
  }
}

// *** 카메라 내부 파라미터 영역 *** //

int CameraPoseEstimator::calculate_intrinsic_parameters()
{
  const float _square_size = 0.02410f;
  const cv::Size _dimensions = cv::Size(8-1, 11-1);
  cv::Mat _frame, _drawn_frame;
  std::vector<cv::Mat> _saved_frames;
  cv::Mat _camera_matrix = cv::Mat::eye(3, 3, CV_64F);
  cv::Mat _distortion_coefficients = cv::Mat::zeros(8, 1, CV_16F);
  std::vector<cv::Mat> _rotation_vectors, _translation_vectors;
  cv::VideoCapture _vid(0);

  // ERROR CODE '-1' >> CAMERA_NOT_OPENED
  if (!_vid.isOpened()) return -1;

  cv::namedWindow("INTRINSIC - CHESSBOARD FOUND", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("INTRINSIC - CHESSBOARD NOT FOUND", cv::WINDOW_AUTOSIZE);

  while (true)
  {
    // ERROR CODE '-2' >> IMAGE_CANNOT_READ
    if (!_vid.read(_frame))
    {
      _vid.release();
      return -2;
    }

    bool _found = false;
    std::vector<cv::Vec2f> _found_points;

    _frame.copyTo(_drawn_frame);

    _found = cv::findChessboardCorners(_frame, _dimensions, _found_points, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK);
    cv::drawChessboardCorners(_drawn_frame, _dimensions, _found_points, _found);

    if (_found) cv::imshow("INTRINSIC - CHESSBOARD FOUND", _drawn_frame);
    else cv::imshow("INTRINSIC - CHESSBOARD NOT FOUND", _frame);

    std::vector<std::vector<cv::Point2f>> _checkbaord_image_space_points;
    std::vector<std::vector<cv::Point3f>> _world_space_corner_points(1);

    char _input = cv::waitKey(1000/frames_per_second);

    switch (_input)
    {
    // KEY `spacebar` :: 이미지를 벡터에 저장
    case ' ':
      if (_found)
      {
        cv::Mat tmp;
        _frame.copyTo(tmp);
        _saved_frames.push_back(tmp);
        ROS_INFO("FRAME SAVED");
      }
      break;
    // KEY `enter` :: 카메라 캘리브레이션 파라미터 계산 및 저장
    case 13:
      if (_saved_frames.size() > 15)
      {
        for (std::vector<cv::Mat>::iterator _iterator = _saved_frames.begin(); _iterator != _saved_frames.end(); _iterator++)
        {
          std::vector<cv::Point2f> _point_buffer;
          bool found = cv::findChessboardCorners(*_iterator, _dimensions, _point_buffer, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

          if (found)
          {
            for (std::vector<cv::Point2f>::iterator _inner_iterator = _point_buffer.begin(); _inner_iterator != _point_buffer.end(); _inner_iterator++)
            {
              _checkbaord_image_space_points.push_back(_point_buffer);
            }
          }
        }
        create_world_coordinate_system(_world_space_corner_points[0]);
        _world_space_corner_points.resize(_checkbaord_image_space_points.size(), _world_space_corner_points[0]);

        ROS_INFO("BEFORE CALIBRATION");
        cv::calibrateCamera(_world_space_corner_points, _checkbaord_image_space_points, _dimensions, _camera_matrix, _distortion_coefficients, _rotation_vectors, _translation_vectors);
        ROS_INFO("AFTER CALIBRATION");

        save_intrinsic_parameters("intrinsic+parameters.txt", _camera_matrix, _distortion_coefficients);
      }
      break;
    // KEY `esc` :: 프로그램 종료
    case 27:
      // _vid.release();
      ROS_INFO("INTRINSIC CALIBRATION COMPLETED");
      return 0;
    }
  }
}

bool CameraPoseEstimator::save_intrinsic_parameters(std::string filename, cv::Mat calculated_camera_matrix, cv::Mat calculated_distortion_coefficients) {
  std::ofstream outStream(filename);
  if (outStream) {
    uint16_t rows = calculated_camera_matrix.rows;
    uint16_t columns = calculated_camera_matrix.cols;

    // outStream << "camera matrix>>>" << std::endl;
    for (int r=0; r<rows; r++) {
      for (int c=0; c<columns; c++) {
        double value = calculated_camera_matrix.at<double>(r, c);
        outStream << value << std::endl;
      }
    }

    rows = calculated_distortion_coefficients.rows;
    columns = calculated_distortion_coefficients.cols;

    // outStream << "distortion coefficients>>>" << std::endl;
    for (int r=0; r<rows; r++) {
      for (int c=0; c<columns; c++) {
        double value = calculated_distortion_coefficients.at<double>(r, c);
        outStream << value << std::endl;
      }
    }

    outStream.close();
    return true;
  }
  return false;
}