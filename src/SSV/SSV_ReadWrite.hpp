/**
 * "SSV_ReadWrite.hpp"
 **/



#ifndef SRC_SSV_SSV_READWRITE_HPP_
#define SRC_SSV_SSV_READWRITE_HPP_



#include <iostream>
#include <stdio.h>
#include <stdlib.h>     // strtol
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h> 		// Contains file controls like O_RDWR
#include <termios.h> 	// Contains POSIX terminal control definitions
#include <unistd.h> 	// write(), read(), close()



#define SSV_SERVO_MESSAGE_MOVE_TIME_WRITE			1	// Implemented > Position value ([0 ; 1000]) = Angle value ([0 deg ; 240 deg])
#define SSV_SERVO_MESSAGE_MOVE_TIME_READ			2
#define SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_WRITE		7
#define SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_READ		8
#define SSV_SERVO_MESSAGE_MOVE_START				11
#define SSV_SERVO_MESSAGE_MOVE_STOP					12
#define SSV_SERVO_MESSAGE_ID_WRITE					13
#define SSV_SERVO_MESSAGE_ID_READ					14
#define SSV_SERVO_MESSAGE_ANGLE_OFFSET_ADJUST		17
#define SSV_SERVO_MESSAGE_ANGLE_OFFSET_WRITE		18
#define SSV_SERVO_MESSAGE_ANGLE_OFFSET_READ			19
#define SSV_SERVO_MESSAGE_ANGLE_LIMIT_WRITE			20
#define SSV_SERVO_MESSAGE_ANGLE_LIMIT_READ			21
#define SSV_SERVO_MESSAGE_VIN_LIMIT_WRITE			22
#define SSV_SERVO_MESSAGE_VIN_LIMIT_READ			23
#define SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_WRITE		24
#define SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_READ		25
#define SSV_SERVO_MESSAGE_TEMP_READ					26	// Implemented > ...
#define SSV_SERVO_MESSAGE_VIN_READ					27	// Implemented > ...
#define SSV_SERVO_MESSAGE_POS_READ					28	// Implemented > Position value ([0 ; 1000]) = Angle value ([0 deg ; 240 deg])
#define SSV_SERVO_MESSAGE_OR_MOTOR_MODE_WRITE		29	// Implemented > Speed value : [-1000 ; 1000]
#define SSV_SERVO_MESSAGE_OR_MOTOR_MODE_READ		30
#define SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_WRITE		31
#define SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_READ		32
#define SSV_SERVO_MESSAGE_LED_CTRL_WRITE			33
#define SSV_SERVO_MESSAGE_LED_CTRL_READ				34
#define SSV_SERVO_MESSAGE_LED_ERROR_WRITE			35
#define SSV_SERVO_MESSAGE_LED_ERROR_READ			36

#define SSV_DEFAULT_DEVICE_SERIAL_PORT_NAME			"/dev/ttyUSB0"
#define SSV_DEFAULT_DEVICE_SERIAL_PORT_BAUD_RATE	115200
#define SSV_PACKET_HEADER_BYTES_SIZE				2
#define SSV_PACKET_SERVO_ID_BYTES_SIZE				1
#define SSV_PACKET_DATA_LENGTH_BYTES_SIZE			1
#define SSV_PACKET_CMD_BYTES_SIZE					1
#define SSV_PACKET_CHECKSUM_BYTES_SIZE				1
#define SSV_ANGLE_DEG_TOL							2
#define SSV_ANGLE_LIMIT_MIN							0
#define SSV_ANGLE_LIMIT_MAX							1000
#define SSV_ROTATION_SPEED_VALUE_LIMIT_MIN			-1000
#define SSV_ROTATION_SPEED_VALUE_LIMIT_MAX			1000
#define SSV_BUFFER_SIZE_MAX							100
#define SSV_PARAMETERS_NB_MAX						4



using namespace std;



namespace SSV
{
	/**
		@class CLx16a
		@brief Serial communication class for LX-16A servo units
	 **/
	class CLx16a
	{
		public:
			/**
				@brief constructor
			 **/
			CLx16a();

			/**
				@brief constructor
				@param[in] p_deviceSerialPortName : device serial port name
				@param[in] p_deviceSerialPorteBaudRate : device serial port baud rate
			 **/
			CLx16a(string p_deviceSerialPortName, uint32_t p_deviceSerialPortBaudRate);

			/**
				@brief destructor
			 **/
			virtual ~CLx16a();

			/**
				@brief method to initialize the serial device port
			 **/
			int initDeviceSerialPort();

			/**
				@brief method to write on the serial device port
			 **/
			int writeDeviceSerialPort(uint32_t p_servoId, uint32_t p_cmd, double* p_parameter);

			/**
				@brief method to read on the serial device port
			 **/
			int readDeviceSerialPort(uint32_t p_servoId, uint32_t p_cmd, void* p_buffer);

			/**
				@brief method to get a command length
			 **/
			uint32_t getCmdLength(uint32_t p_cmd);

			/**
				@brief method to get a return command length
			 **/
			uint32_t getRetCmdLength(uint32_t p_cmd);

			/**
				@brief method to convert command parameters
			 **/
			uint32_t convertCmdParameters(uint32_t p_cmd, uint32_t p_cmdLengthInBytes, unsigned char* p_bufferToConvert, void* p_buffer);

			/**
				@brief method to get a command checksum
			 **/
			uint32_t getCmdChecksum(unsigned char* p_buffer, uint32_t p_bufferBytesSize);

			/**
				@brief method to set command parameters
			 **/
			int setCmdParameters(uint32_t p_cmd, unsigned char* p_buffer, double* p_parameter);

		private:
			string		m_deviceSerialPortName;
			uint32_t	m_deviceSerialPortBaudRate;
			int 		m_deviceSerialPort;
	};
}



#endif /* SRC_SSV_SSV_READWRITE_HPP_ */
