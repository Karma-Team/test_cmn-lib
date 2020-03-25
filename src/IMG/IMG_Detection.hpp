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
				@brief destructor
			 **/
			virtual ~CDetection();

			/**
				@brief method for cups detection
			 **/
			void cupsDetection(Mat& inImgCamera);

			/**
				@brief method for aruco codes detection
			 **/
			void arucoCodesDetection();

			/**
				@brief method for robots detection
			 **/
			void robotsDetection();

			/**
				@brief method for merging detections
			 **/
			void mergeDetections();

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
			Mat 			m_inImgCamera;							//< IN image (from camera)
			Mat 			m_tmpInImgCameraHsv;					//< IN image (from camera, converted in HSV)
			Mat 			m_outImgCupsDetection;					//< OUT image (from cups detector thread)
			Mat 			m_outImgRobotsDetection[NB_ROBOTS_MAX];	//< OUT image (from robots detector thread)
			Mat 			m_outImgMergeDectections;				//< OUT image (from merge detections)
			uint32_t		m_detectedRobotsNb;						//< Detected robots number
	};
}



#endif /* INC_IMG_DETECTION_HPP_ */
