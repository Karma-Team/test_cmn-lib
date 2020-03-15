/*
 * SPI_ReadWrite.cpp
 *
 *  Created on: 22 janv. 2020
 *      Author: thomas
 */

#include "SPI_ReadWrite.hpp"


SPI::CSpiReadWrite::CSpiReadWrite(uint8_t p_channel, uint32_t p_speed)
{
	m_channel = p_channel;
	m_setupSpi = wiringPiSPISetup(m_channel, p_speed);
	(void)memset(m_buffer, 0, 3);
}

SPI::CSpiReadWrite::~CSpiReadWrite()
{
	// rien a faire
}

void SPI::CSpiReadWrite::write(uint8_t p_addr, uint16_t p_data)
{
	m_buffer[0] = 0x7f & p_addr;
	m_buffer[1] = p_data>>8;
	m_buffer[2] = p_data&0xff;

	(void)wiringPiSPIDataRW(m_channel, m_buffer, 3);

}

uint16_t SPI::CSpiReadWrite::read(uint8_t p_addr)
{
	m_buffer[0] = 0x80 | (0x7f & p_addr);

	(void)wiringPiSPIDataRW(m_channel, m_buffer, 3);

	uint16_t data = 0;
	data = static_cast<uint16_t>(static_cast<uint8_t>(m_buffer[1]) + static_cast<uint8_t>(m_buffer[2]));

	return data;

}
