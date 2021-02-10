/**
 * "common.hpp"
 **/

#ifndef INC_COMMON_HPP_
#define INC_COMMON_HPP_

#include <cstdint>

#define MAX_PATH_POINTS	10
#define LOG_ON			1

struct SPoint {
	int16_t	x;
	int16_t y;
};

inline void logDebug( std::string message ) {
	#if LOG_ON
	std::cout << message << std::endl;
	#endif
}

#endif /* INC_COMMON_HPP_ */
