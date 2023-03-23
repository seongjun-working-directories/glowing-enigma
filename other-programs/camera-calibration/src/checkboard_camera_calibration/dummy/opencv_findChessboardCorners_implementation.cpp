/*
bool findChessboardCorners(InputArray image_, Size pattern_size, OutputArray corners_, int flags)
{
  CV_INSTRUMENT_REGION();

  DPRINTF("==== findChessboardCorners(img=%dx%d, pattern=%dx%d, flags=%d)",
    image_.cols(), image_.rows(), pattern_size.width, pattern_size.height, flags);

  bool found = false;

  const int min_dilations = 0;
  const int max_dilations = 7;

  int type = image_.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
  Mat img = image_.getMat();

  CV_CheckType(type, depth == CV_8U && (cn == 1 || cn == 3 || cn == 4),
    "Only 8-bit grayscale or color images are supported");

  if (pattern_size.width <= 2 || pattern_size.height <= 2)
    CV_Error(Error::StsOutOfRange, "Both width and height of the pattern should have bigger than 2");

  if (!corners_.needed())
    CV_Error(Error::StsNullPtr, "Null pointer to corners");

  std::vector<cv::Point2f> out_corners;

  if (img.channels() != 1)
  {
    cvtColor(img, img, COLOR_BGR2GRAY);
  }

  int prev_sqr_size = 0;

  Mat thresh_img_new = img.clone();
  icvBinarizationHistogramBased(thresh_img_new); // process image in-place
  SHOW("New binarization", thresh_img_new);

  if (flags & CALIB_CB_FAST_CHECK)
  {
    //perform new method for checking chessboard using a binary image.
    //image is binarised using a threshold dependent on the image histogram
    if (checkChessboardBinary(thresh_img_new, pattern_size) <= 0) //fall back to the old method
    {
      if (!checkChessboard(img, pattern_size))
      {
        corners_.release();
        return false;
      }
    }
  }

  ChessBoardDetector detector(pattern_size);

  // Try our standard "1" dilation, but if the pattern is not found, iterate the whole procedure with higher dilations.
  // This is necessary because some squares simply do not separate properly with a single dilation.  However,
  // we want to use the minimum number of dilations possible since dilations cause the squares to become smaller,
  // making it difficult to detect smaller squares.
  for (int dilations = min_dilations; dilations <= max_dilations; dilations++)
  {
    //USE BINARY IMAGE COMPUTED USING icvBinarizationHistogramBased METHOD
    dilate( thresh_img_new, thresh_img_new, Mat(), Point(-1, -1), 1 );

    // So we can find rectangles that go to the edge, we draw a white line around the image edge.
    // Otherwise FindContours will miss those clipped rectangle contours.
    // The border color will be the image mean, because otherwise we risk screwing up filters like cvSmooth()...
    rectangle( thresh_img_new, Point(0,0), Point(thresh_img_new.cols-1, thresh_img_new.rows-1), Scalar(255,255,255), 3, LINE_8);

    detector.reset();

#ifdef USE_CV_FINDCONTOURS
    Mat binarized_img = thresh_img_new;
#else
    Mat binarized_img = thresh_img_new.clone(); // make clone because cvFindContours modifies the source image
#endif
    detector.generateQuads(binarized_img, flags);
    DPRINTF("Quad count: %d/%d", detector.all_quads_count, (pattern_size.width/2+1)*(pattern_size.height/2+1));
    SHOW_QUADS("New quads", thresh_img_new, &detector.all_quads[0], detector.all_quads_count);
    if (detector.processQuads(out_corners, prev_sqr_size))
    {
      found = true;
      break;
    }
  }

  DPRINTF("Chessboard detection result 0: %d", (int)found);

  // revert to old, slower, method if detection failed
  if (!found)
  {
    if (flags & CALIB_CB_NORMALIZE_IMAGE)
    {
      img = img.clone();
      equalizeHist(img, img);
    }

    Mat thresh_img;
    prev_sqr_size = 0;

    DPRINTF("Fallback to old algorithm");
    const bool useAdaptive = flags & CALIB_CB_ADAPTIVE_THRESH;
    if (!useAdaptive)
    {
      // empiric threshold level
      // thresholding performed here and not inside the cycle to save processing time
      double mean = cv::mean(img).val[0];
      int thresh_level = std::max(cvRound(mean - 10), 10);
      threshold(img, thresh_img, thresh_level, 255, THRESH_BINARY);
    }

    //if flag CALIB_CB_ADAPTIVE_THRESH is not set it doesn't make sense to iterate over k
    int max_k = useAdaptive ? 6 : 1;
    for (int k = 0; k < max_k && !found; k++)
    {
      for (int dilations = min_dilations; dilations <= max_dilations; dilations++)
      {
        // convert the input grayscale image to binary (black-n-white)
        if (useAdaptive)
        {
          int block_size = cvRound(prev_sqr_size == 0
            ? std::min(img.cols, img.rows) * (k % 2 == 0 ? 0.2 : 0.1)
            : prev_sqr_size * 2);
          block_size = block_size | 1;
          // convert to binary
          adaptiveThreshold( img, thresh_img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, block_size, (k/2)*5 );
          if (dilations > 0)
            dilate( thresh_img, thresh_img, Mat(), Point(-1, -1), dilations-1 );
        }
        else
        {
            dilate( thresh_img, thresh_img, Mat(), Point(-1, -1), 1 );
        }
        SHOW("Old binarization", thresh_img);

        // So we can find rectangles that go to the edge, we draw a white line around the image edge.
        // Otherwise FindContours will miss those clipped rectangle contours.
        // The border color will be the image mean, because otherwise we risk screwing up filters like cvSmooth()...
        rectangle( thresh_img, Point(0,0), Point(thresh_img.cols-1, thresh_img.rows-1), Scalar(255,255,255), 3, LINE_8);

        detector.reset();

#ifdef USE_CV_FINDCONTOURS
        Mat binarized_img = thresh_img;
#else
        Mat binarized_img = (useAdaptive) ? thresh_img : thresh_img.clone(); // make clone because cvFindContours modifies the source image
#endif
        detector.generateQuads(binarized_img, flags);
        DPRINTF("Quad count: %d/%d", detector.all_quads_count, (pattern_size.width/2+1)*(pattern_size.height/2+1));
        SHOW_QUADS("Old quads", thresh_img, &detector.all_quads[0], detector.all_quads_count);
        if (detector.processQuads(out_corners, prev_sqr_size))
        {
          found = 1;
          break;
        }
      }
    }
  }

  DPRINTF("Chessboard detection result 1: %d", (int)found);

  if (found)
    found = detector.checkBoardMonotony(out_corners);

  DPRINTF("Chessboard detection result 2: %d", (int)found);

  // check that none of the found corners is too close to the image boundary
  if (found)
  {
    const int BORDER = 8;
    for (int k = 0; k < pattern_size.width*pattern_size.height; ++k)
    {
      if( out_corners[k].x <= BORDER || out_corners[k].x > img.cols - BORDER ||
        out_corners[k].y <= BORDER || out_corners[k].y > img.rows - BORDER )
      {
        found = false;
        break;
      }
    }
  }

  DPRINTF("Chessboard detection result 3: %d", (int)found);

  if (found)
  {
    if ((pattern_size.height & 1) == 0 && (pattern_size.width & 1) == 0 )
    {
      int last_row = (pattern_size.height-1)*pattern_size.width;
      double dy0 = out_corners[last_row].y - out_corners[0].y;
      if (dy0 < 0)
      {
        int n = pattern_size.width*pattern_size.height;
        for(int i = 0; i < n/2; i++ )
        {
          std::swap(out_corners[i], out_corners[n-i-1]);
        }
      }
    }
    cv::cornerSubPix(
      img, out_corners, Size(2, 2), Size(-1,-1),
      cv::TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 15, 0.1)
    );
  }

  Mat(out_corners).copyTo(corners_);
  return found;
}
*/
