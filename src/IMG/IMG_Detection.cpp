/**
 * "IMG_Detection.cpp"
 **/



#include "IMG_Detection.hpp"



IMG::CDetection::CDetection()
{
	m_detectedRobotsNb 	= 0;
}



IMG::CDetection::~CDetection()
{
}



void IMG::CDetection::cupsDetection(Mat& imgInImgCamera)
{
	Mat imgInImgCameraHsv;
	Mat imgThresholdedGreen;
	Mat imgThresholdedRed1;
	Mat imgThresholdedRed2;
	Mat imgThresholded;

	// Convert a video frame from BGR to HSV
		cvtColor(imgInImgCamera, imgInImgCameraHsv, COLOR_BGR2HSV);

	// Threshold the image : inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded)
		inRange(imgInImgCameraHsv, Scalar(HL_GREEN, 	SL_GREEN, 	VL_GREEN),	Scalar(HH_GREEN, 	SH_GREEN, 	VH_GREEN),	imgThresholdedGreen);
		inRange(imgInImgCameraHsv, Scalar(HL_RED1, 	SL_RED1, 	VL_RED1),	Scalar(HH_RED1, 	SH_RED1, 	VH_RED1),	imgThresholdedRed1);
		inRange(imgInImgCameraHsv, Scalar(HL_RED2, 	SL_RED2, 	VL_RED2),	Scalar(HH_RED2, 	SH_RED2, 	VH_RED2),	imgThresholdedRed2);
		imgThresholded 	= imgThresholdedGreen + imgThresholdedRed1 + imgThresholdedRed2;

	// Morphological opening (remove small objects from the foreground)
		erode(imgThresholded,	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, 	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	// Morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, 	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, 	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	// Save the output image
		m_outImgCupsDetection = imgThresholded.clone();

	// FOR DEBUG ONLY (begin)
	// Show the original and the thresholded image
		imshow("Original Image", 		imgInImgCamera);
		imshow("Original Image HSV",	imgInImgCameraHsv);
		imshow("Thresholded Image",		imgThresholded);

		// Wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if(waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			return;
		}
	// FOR DEBUG ONLY (end)
}



void IMG::CDetection::arucoCodesDetection()
{
}



void IMG::CDetection::robotsDetection()
{
}



void IMG::CDetection::mergeDetections()
{
}
