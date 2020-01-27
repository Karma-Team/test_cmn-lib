/*
 * SPI_ReadWrite.hpp
 *
 *  Created on: 22 janv. 2020
 *      Author: thomas
 */

#ifndef SRC_SPI_SPI_READWRITE_HPP_
#define SRC_SPI_SPI_READWRITE_HPP_

#include <stdint.h>
//#include <wiringPiSPI.h>

namespace SPI
{
	/**
	 * @class CSpiReadWrite
	 * @brief classe de la communication spi
	 */
	class CSpiReadWrite
	{
	public :
		CSpiReadWrite(uint8_t p_channel, uint32_t p_speed);

		~CSpiReadWrite();

		void write(uint8_t p_addr, uint16_t p_data);

		void read();

	private :

		uint8_t m_channel;		 //< Channel du lien spi
		uint32_t m_speed;		 //< Vitesse de liaison spi


	};

}


#endif /* SRC_SPI_SPI_READWRITE_HPP_ */
