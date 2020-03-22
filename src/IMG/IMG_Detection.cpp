/**
 * "IMG_Detection.cpp"
 **/



#include "IMG_Detection.hpp"



IMG::CDetection::CDetection()
{
	m_videoCapture.open(0);			//< Open the default video capturing device
	m_detectedRobotsNb 	= 0;
	m_detectionStatus	= true;
	m_readVideoSuccess 	= false;
}



IMG::CDetection::CDetection(const String& filename)
{
	m_videoCapture.open(filename);	//< Open the given video file
	m_detectedRobotsNb 	= 0;
	m_detectionStatus	= true;
	m_readVideoSuccess 	= false;
}



IMG::CDetection::~CDetection()
{
}



void IMG::CDetection::cupsAndArucoCodesDetectionProcess()
{
	// Check if the video is well opened
	if(!m_videoCapture.isOpened())
	{
		cout << "Cannot open the the video" << endl;
		return;
	}
	else
	{
		// Launch the read video thread
			m_readVideoThread 			= thread(&IMG::CDetection::readVideoThread,	 			this);

		// Launch the cups detector thread
			m_cupsDetectionThread 		= thread(&IMG::CDetection::cupsDetectionThread, 		this);

			/*
		// Launch the aruco codes detector thread
			m_arucoCodesDetectionThread	= thread(&IMG::CDetection::arucoCodesDetectionThread, 	this);

		// Launch the merge detections thread
			m_mergeDetectionsThread 	= thread(&IMG::CDetection::mergeDetectionsThread, 		this);
			*/

		// Loop
			while(m_detectionStatus == true);
	}

	return;
}



void IMG::CDetection::readVideoThread()
{
	while(m_detectionStatus == true)
	{
		// Read a video frame
			m_tmpInImgCameraHsvMutex.lock();
			m_readVideoSuccess = m_videoCapture.read(m_inImgCamera);

		// Convert the captured frame from BGR to HSV
			if(m_readVideoSuccess == true)
			{
				cvtColor(m_inImgCamera, m_tmpInImgCameraHsv, COLOR_BGR2HSV);
			}
			m_tmpInImgCameraHsvMutex.unlock();
	}
}



void IMG::CDetection::cupsDetectionThread()
{
	Mat imgThresholdedGreen;
	Mat imgThresholdedRed1;
	Mat imgThresholdedRed2;
	Mat imgThresholded;

	while(m_detectionStatus == true)
	{
		if(m_readVideoSuccess == true)
		{
			// Threshold the image : inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded)
				m_tmpInImgCameraHsvMutex.lock();
				inRange(m_tmpInImgCameraHsv, Scalar(HL_GREEN, 	SL_GREEN, 	VL_GREEN),	Scalar(HH_GREEN, 	SH_GREEN, 	VH_GREEN),	imgThresholdedGreen);
				inRange(m_tmpInImgCameraHsv, Scalar(HL_RED1, 	SL_RED1, 	VL_RED1),	Scalar(HH_RED1, 	SH_RED1, 	VH_RED1),	imgThresholdedRed1);
				inRange(m_tmpInImgCameraHsv, Scalar(HL_RED2, 	SL_RED2, 	VL_RED2),	Scalar(HH_RED2, 	SH_RED2, 	VH_RED2),	imgThresholdedRed2);
				m_tmpInImgCameraHsvMutex.unlock();
				imgThresholded 	= imgThresholdedGreen + imgThresholdedRed1 + imgThresholdedRed2;

			// Morphological opening (remove small objects from the foreground)
				erode(imgThresholded,	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				dilate(imgThresholded, 	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			// Morphological closing (fill small holes in the foreground)
				dilate(imgThresholded, 	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				erode(imgThresholded, 	imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			// Save the output image
				m_outImgCupsDetection = imgThresholded.clone();


			///////////////////////////////////////////////
				// Show the original and the thresholded image
				imshow("Thresholded Image",	imgThresholded);
				imshow("Original Image", 	m_inImgCamera);

				// Wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				if(waitKey(30) == 27)
				{
					cout << "esc key is pressed by user" << endl;
					break;
				}
		}
	}
}



void IMG::CDetection::arucoCodesDetectionThread()
{
	while(m_detectionStatus == true)
	{
		m_detectedRobotsNb = 0;

		// Launch a robot detector thread
			m_robotsDetectionThread[m_detectedRobotsNb] = thread(&IMG::CDetection::robotsDetectionThread, this);
			m_detectedRobotsNb++;

		// Wait for all robotsDetectionThread to finish
	}
}



void IMG::CDetection::robotsDetectionThread()
{
	while(m_detectionStatus == true)
	{

	}
}





void IMG::CDetection::mergeDetectionsThread()
{
	while(m_detectionStatus == true)
	{
		// Wait for cupsDetectionThread and all robotsDetectionThread to finish before merging the mask

	}
}
