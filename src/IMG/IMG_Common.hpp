/**
 * "IMG_Common.hpp"
 **/



#ifndef INC_IMG_COMMON_HPP_
#define INC_IMG_COMMON_HPP_



#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/flann.hpp>
#include <opencv2/gapi.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>



#define NB_ROBOTS_MAX	4
#define HL_GREEN		30
#define HH_GREEN		70
#define SL_GREEN		75
#define SH_GREEN		255
#define VL_GREEN		0
#define VH_GREEN		255
#define HL_RED1			0
#define HH_RED1			10
#define SL_RED1			100
#define SH_RED1			255
#define VL_RED1			100
#define VH_RED1			255
#define HL_RED2			160
#define HH_RED2			180
#define SL_RED2			100
#define SH_RED2			255
#define VL_RED2			100
#define VH_RED2			255



using namespace std;
using namespace cv;



#endif /* INC_IMG_COMMON_HPP_ */
