/*
 * SPI_ReadWrite.cpp
 *
 *  Created on: 22 janv. 2020
 *      Author: thomas
 */

#include "SPI_ReadWrite.hpp"


SPI::CSpiReadWrite::CSpiReadWrite(uint8_t p_channel, uint32_t p_speed)
{
	m_channel = 0;
	m_speed = 0;
}
