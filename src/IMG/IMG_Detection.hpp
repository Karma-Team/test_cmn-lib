/**
 * "IMG_Detection.hpp"
 **/



#ifndef INC_IMG_DETECTION_HPP_
#define INC_IMG_DETECTION_HPP_



#include "IMG_Common.hpp"



using namespace std;



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
	};
}



#endif /* INC_IMG_DETECTION_HPP_ */
