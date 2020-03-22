/**
 * "IMG_Detection.hpp"
 **/



#ifndef INC_IMG_DETECTION_HPP_
#define INC_IMG_DETECTION_HPP_



#include "IMG_Common.hpp"



namespace IMG
{
	/**
		@class CDetection
		@brief Image processing (detection) class
	 **/
	class CDetection
	{
		public:
			/**
				@brief constructor
			 **/
			CDetection();

			/**
				@brief constructor
			 **/
			CDetection(const String& filename);

			/**
				@brief destructor
			 **/
			virtual ~CDetection();

			/**
				@brief thread-method to launch the detection process of cups and aruco codes
			 **/
			void cupsAndArucoCodesDetectionProcess();

			/**
				@brief thread-method for read video
			 **/
			void readVideoThread();

			/**
				@brief thread-method for cups detection
			 **/
			void cupsDetectionThread();

			/**
				@brief thread-method for aruco codes detection
			 **/
			void arucoCodesDetectionThread();

			/**
				@brief thread-method for robots detection
			 **/
			void robotsDetectionThread();

			/**
				@brief thread-method for merging detections
			 **/
			void mergeDetectionsThread();

			/**
				@brief method for getting the IN image (from camera)
			 **/
			void getInImgCamera();

			/**
				@brief method for getting the OUT image (from cups detection thread)
			 **/
			void getOutImgCupsDetection();

			/**
				@brief method for getting the OUT image (from robots detection thread)
			 **/
			void getOutImgRobotsDetection();

			/**
				@brief method for getting the OUT image (from merge detections)
			 **/
			void getOutImgMergeDetections();

		private:
			thread 			m_readVideoThread;						//< Read video thread
			thread 			m_cupsDetectionThread;					//< Cups detection thread
			thread 			m_arucoCodesDetectionThread;			//< Aruco codes detection thread
			thread 			m_robotsDetectionThread[NB_ROBOTS_MAX];	//< Robots detection threads
			thread 			m_mergeDetectionsThread;				//< Merge detections thread
			mutex 			m_tmpInImgCameraHsvMutex;				//< IN image mutex
			VideoCapture 	m_videoCapture;							//< Video capture (input)
			Mat 			m_inImgCamera;							//< IN image (from camera)
			Mat 			m_tmpInImgCameraHsv;					//< IN image (from camera, converted in HSV)
			Mat 			m_outImgCupsDetection;					//< OUT image (from cups detector thread)
			Mat 			m_outImgRobotsDetection[NB_ROBOTS_MAX];	//< OUT image (from robots detector thread)
			Mat 			m_outImgMergeDectections;				//< OUT image (from merge detections)
			uint32_t		m_detectedRobotsNb;						//< Detected robots number
			bool			m_detectionStatus;						//< Detection status
			bool			m_readVideoSuccess;						//< Read video success indicator
	};
}



#endif /* INC_IMG_DETECTION_HPP_ */
