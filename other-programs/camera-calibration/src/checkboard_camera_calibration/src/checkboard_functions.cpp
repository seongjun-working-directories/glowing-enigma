// *** aruco marker 생성 함수 *** //
/*
void aruco_marker_creator() {
  cv::Mat outputMarker;

  cv::Ptr<cv::aruco::Dictionary> markerDictionary
    = cv::aruco::getPredefinedDictionary(
      cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_100
    );

  for (int i=0; i<100; i++) {
    cv::aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
    std::ostringstream convert;
    std::string imageName = "4X4 Marker - ";
    convert << imageName << i << ".jpg";
    cv::imwrite(convert.str(), outputMarker);
  }
}
*/

// *** 체크보드의 코너값을 저장하는 함수  *** //
void getCheckboardCorners(
  std::vector<cv::Mat> images, std::vector<std::vector<cv::Point2f>>& allFoundCorners, bool showResults = false
) {
  // 루프를 돌며 각 이미지마다 연산을 처리
  for (std::vector<cv::Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
    std::vector<cv::Point2f> pointBuf;
    bool found = cv::findChessboardCorners(
      *iter, cv::Size(8-1, 11-1), pointBuf, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE
    );

    // cv::findChessboardCorners를 통해 찾아낸 Corner Points들을 allFoundCorners 변수에 저장
    if (found) {
      // [TEST] pointBuf에 코너 좌표가 잘 들어오는지 확인
      for (std::vector<cv::Point2f>::iterator iter = pointBuf.begin(); iter != pointBuf.end(); iter++) {
        ROS_INFO("pointBuf>>> (%f, %f)", (*iter).x, (*iter).y);
      }

      allFoundCorners.push_back(pointBuf);
    }
    if (showResults) {
      cv::drawChessboardCorners(*iter, cv::Size(8-1, 11-1), pointBuf, found);
      cv::imshow("Looking for corners", *iter);
      cv::waitKey(0);
    }
  }
}

// *** 체크보드의 실제 크기를 좌표값으로 저장하는 함수 *** //
void createKnownBoardPosition(
  cv::Size boardSize, float squaredEdgeLength, std::vector<cv::Point3f>& corners
) {
  for (int i=0; i<boardSize.height; i++) {
    for (int j=0; j<boardSize.width; j++) {
      corners.push_back(
        // 체크보드는 평면이기 때문에 z값이 0인 것이 특징
        cv::Point3f(j*squaredEdgeLength, i*squaredEdgeLength, 0.0f)
      );
    }
  }
}

// *** 카메라 파라미터 계수를 구하는 함수 *** //
void cameraCalibration(std::vector<cv::Mat> calibrationImages, cv::Size boardSize, float squareEdgeLength, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients) {
  // 촬영한 체크보드(calibrationImages)와 2차원 벡터(checkboardImageSpacePoints)를 인자로 전달
  // getCheckboardCorners 함수를 통해 checkboardImageSpacePoints에 각 이미지의 코너값을 저장함
  std::vector<std::vector<cv::Point2f>> checkboardImageSpacePoints;
  getCheckboardCorners(calibrationImages, checkboardImageSpacePoints, false);

  std::vector<std::vector<cv::Point3f>> worldSpaceCornerPoints(1);

  // 체크보드의 가로, 세로 및 각 칸의 길이, 3차원 좌표로 이뤄진 단일 벡터를 인자로 전달
  createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);

  // std::vector::size - https://cplusplus.com/reference/vector/vector/size/
  // std::vector::resize - https://cplusplus.com/rehttps://cplusplus.com/reference/vector/vector/resize/ference/vector/vector/resize/
  worldSpaceCornerPoints.resize(checkboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

  std::vector<cv::Mat> rVectors, tVectors;
  distanceCoefficients = cv::Mat::zeros(8, 1, CV_16F);

  // cv::calibrateCamera : finds the camera intrinsic & extrinsic parameters from serveral views of a calibration pattern
  // (링크 : https://amroamroamro.github.io/mexopencv/matlab/cv.calibrateCamera.html)
  cv::calibrateCamera(worldSpaceCornerPoints, checkboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficients, rVectors, tVectors);

  // rotation & translation vector 관련 자료는 `Rotate_Translate_Vector.pdf` 파일을 확인

  /*
  [ cv::calibrateCamera ]
  01. cameraMartix
  -> Input/output 3x3 floating-point camera intrinsic matrix A = ⎡ fx   0  cx  ⎤
                                                                 ⎢  0  fy  cy  ⎥
                                                                 ⎣  0   0   1  ⎦ .
  02. distanceCoefficients --> (k1, k2, p1, p2, k3)
  -> Input/output vector of distortion coefficients (k1,k2,p1,p2[,k3[,k4,k5,k6[,s1,s2,s3,s4[,τx,τy]]]]) of 4, 5, 8, 12 or 14 elements.
  */
}

// *** 카메라 파라미터 계수를 저장하는 함수 *** //
bool saveCameraCalibration(std::string name, cv::Mat cameraMatrix, cv::Mat distanceCoefficients) {
  std::ofstream outStream(name);
  if (outStream) {
    uint16_t rows = cameraMatrix.rows;
    uint16_t columns = cameraMatrix.cols;

    // [TEST] outStream << "cameraMatrix>>>" << std::endl;

    for (int r=0; r<rows; r++) {
      for (int c=0; c<columns; c++) {
        double value = cameraMatrix.at<double>(r, c);
        outStream << value << std::endl;
      }
    }

    rows = distanceCoefficients.rows;
    columns = distanceCoefficients.cols;

    // [TEST] outStream << "distanceCoefficients>>>" << std::endl;

    for (int r=0; r<rows; r++) {
      for (int c=0; c<columns; c++) {
        double value = distanceCoefficients.at<double>(r, c);
        // [TEST] std::cout << "(" << r << ", " << c << ")" << " = " << value << std::endl;

        outStream << value << std::endl;
      }
    }

    outStream.close();
    return true;
  }
  return false;
}

// *** 카메라 왜곡 보정 *** //
/*
Undistort(왜곡 보정)를 하기 위해서는 checkboard_camera_calibrator에서 저장한 파라미터와
opencv4의 getOptimalNewCameraMatrix() 및 undistort() 함수를 사용해야 함
*/