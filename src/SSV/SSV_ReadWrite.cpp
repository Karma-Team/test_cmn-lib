/**
 * "SSV_ReadWrite.cpp"
 **/



#include "SSV_ReadWrite.hpp"



SSV::CLx16a::CLx16a()
{
	m_deviceSerialPortName 		= SSV_DEFAULT_DEVICE_SERIAL_PORT_NAME;
	m_deviceSerialPortBaudRate	= SSV_DEFAULT_DEVICE_SERIAL_PORT_BAUD_RATE;
	m_deviceSerialPort			= -1;
}



SSV::CLx16a::CLx16a(string p_deviceSerialPortName, uint32_t p_deviceSerialPorteBaudRate)
{
	m_deviceSerialPortName 		= p_deviceSerialPortName;
	m_deviceSerialPortBaudRate	= p_deviceSerialPorteBaudRate;
	m_deviceSerialPort			= -1;
}



SSV::CLx16a::~CLx16a()
{
}



int SSV::CLx16a::initDeviceSerialPort()
{
	struct termios	l_tty;
	int 			l_baudRate;

	// Open the device serial port
		m_deviceSerialPort = open(m_deviceSerialPortName.c_str(), O_RDWR);
		if(m_deviceSerialPort < 0)
		{
			return -1;
		}

	// Configure the device serial port
		// Get the existing configuration
			memset(&l_tty, 0, sizeof(l_tty));
			if(tcgetattr(m_deviceSerialPort, &l_tty) != 0)
			{
				return -1;
			}

		// Modify the configuration parameters as needed
			// Clear parity bit, disabling parity (most common)
				l_tty.c_cflag &= ~PARENB;

			// Clear stop field, only one stop bit used in communication (most common)
				l_tty.c_cflag &= ~CSTOPB;

			// 8 bits per byte (most common)
				l_tty.c_cflag |= CS8;

			// Disable RTS/CTS hardware flow control (most common)
				l_tty.c_cflag &= ~CRTSCTS;

			// Turn on READ & ignore ctrl lines (CLOCAL = 1)
				l_tty.c_cflag |= CREAD | CLOCAL;

			// Disabling Canonical Mode
				l_tty.c_lflag &= ~ICANON;

			// Disable echo
				l_tty.c_lflag &= ~ECHO;

			// Disable erasure
				l_tty.c_lflag &= ~ECHOE;

			// Disable new-line echo
				l_tty.c_lflag &= ~ECHONL;

			// Disable interpretation of INTR, QUIT and SUSP
				l_tty.c_lflag &= ~ISIG;

			// Turn off s/w flow ctrl
				l_tty.c_iflag &= ~(IXON | IXOFF | IXANY);

			// Disable any special handling of received bytes
				l_tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

			// Prevent special interpretation of output bytes (e.g. newline chars)
				l_tty.c_oflag &= ~OPOST;

			// Prevent conversion of newline to carriage return/line feed
				l_tty.c_oflag &= ~ONLCR;

			// Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
				// l_tty.c_oflag &= ~OXTABS;

			// Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)
				// l_tty.c_oflag &= ~ONOEOT;

			// Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
				l_tty.c_cc[VTIME] 	= 10;
				l_tty.c_cc[VMIN] 	= 0;

			// Set in/out baud rate
				switch(m_deviceSerialPortBaudRate)
				{
					case 0:
						l_baudRate = B0;
						break;

					case 50:
						l_baudRate = B50;
						break;

					case 75:
						l_baudRate = B75;
						break;

					case 110:
						l_baudRate = B110;
						break;

					case 134:
						l_baudRate = B134;
						break;

					case 150:
						l_baudRate = B150;
						break;

					case 200:
						l_baudRate = B200;
						break;

					case 300:
						l_baudRate = B300;
						break;

					case 600:
						l_baudRate = B600;
						break;

					case 1200:
						l_baudRate = B1200;
						break;

					case 1800:
						l_baudRate = B1800;
						break;

					case 2400:
						l_baudRate = B2400;
						break;

					case 4800:
						l_baudRate = B4800;
						break;

					case 9600:
						l_baudRate = B9600;
						break;

					case 19200:
						l_baudRate = B19200;
						break;

					case 38400:
						l_baudRate = B38400;
						break;

					case 57600:
						l_baudRate = B57600;
						break;

					case 115200:
						l_baudRate = B115200;
						break;

					case 230400:
						l_baudRate = B230400;
						break;

					case 460800:
						l_baudRate = B460800;
						break;

					case 500000:
						l_baudRate = B500000;
						break;

					case 576000:
						l_baudRate = B576000;
						break;

					case 921600:
						l_baudRate = B921600;
						break;

					case 1000000:
						l_baudRate = B1000000;
						break;

					case 1152000:
						l_baudRate = B1152000;
						break;

					case 1500000:
						l_baudRate = B1500000;
						break;

					case 2000000:
						l_baudRate = B2000000;
						break;

					case 2500000:
						l_baudRate = B2500000;
						break;

					case 3000000:
						l_baudRate = B3000000;
						break;

					case 3500000:
						l_baudRate = B3000000;
						break;

					case 4000000:
						l_baudRate = B4000000;
						break;

					default:
						break;
				}
				cfsetispeed(&l_tty, l_baudRate);
				cfsetospeed(&l_tty, l_baudRate);

		// Set the new configuration settings
			if(tcsetattr(m_deviceSerialPort, TCSANOW, &l_tty) != 0)
			{
				return -1;
			}

	return 1;
}



int SSV::CLx16a::writeDeviceSerialPort(uint32_t p_servoId, uint32_t p_cmd, double* p_parameter)
{
	uint32_t 		l_cmdLengthInBytes;		//< Include : data length, command value, command parameters and checksum
	uint32_t 		l_parametersBytesSize;
	uint32_t 		l_bufferBytesSize;
	unsigned char 	l_buffer[SSV_BUFFER_SIZE_MAX];

	// Determine the buffer size
		l_cmdLengthInBytes 		= getCmdLength(p_cmd);
		l_parametersBytesSize 	= l_cmdLengthInBytes - (SSV_PACKET_DATA_LENGTH_BYTES_SIZE + SSV_PACKET_CMD_BYTES_SIZE + SSV_PACKET_CHECKSUM_BYTES_SIZE);
		l_bufferBytesSize 		= SSV_PACKET_HEADER_BYTES_SIZE + SSV_PACKET_SERVO_ID_BYTES_SIZE + SSV_PACKET_DATA_LENGTH_BYTES_SIZE + SSV_PACKET_CMD_BYTES_SIZE + l_parametersBytesSize + SSV_PACKET_CHECKSUM_BYTES_SIZE;

	// File the buffer
		// Header (Bytes 0 -> 1) : 0x55 0x55 = 85 85
			l_buffer[0] = 85;
			l_buffer[1] = 85;

		// Servo ID (Byte 2)
			l_buffer[2] = p_servoId;

		// Data length (Byte 3)
			l_buffer[3] = l_cmdLengthInBytes;

		// Command (Byte 4)
			l_buffer[4] = p_cmd;

		// Parameters (Bytes 5 ? --> ?)
			setCmdParameters(p_cmd, l_buffer, p_parameter);

		// Checksum (Last Byte)
			l_buffer[l_bufferBytesSize-1] = getCmdChecksum(l_buffer, l_bufferBytesSize);

	// Write on the device serial port
		write(m_deviceSerialPort, l_buffer, l_bufferBytesSize);

	return 1;
}



int SSV::CLx16a::readDeviceSerialPort(uint32_t p_servoId, uint32_t p_cmd, void* p_buffer)
{
	uint32_t 		l_retCmdLengthInBytes;		//< Include : data length, command value, command parameters and checksum
	uint32_t 		l_cmdLengthInBytes;			//< Include : data length, command value, command parameters and checksum
	uint32_t 		l_parametersBytesSize;
	uint32_t 		l_bufferBytesSize;
	int		 		l_readBytesNb;
	unsigned char 	l_buffer[SSV_BUFFER_SIZE_MAX];
	double		 	l_parameters[SSV_PARAMETERS_NB_MAX];

	// Determine the buffer size
		l_retCmdLengthInBytes 	= getRetCmdLength(p_cmd);
		l_parametersBytesSize 	= l_retCmdLengthInBytes - (SSV_PACKET_DATA_LENGTH_BYTES_SIZE + SSV_PACKET_CMD_BYTES_SIZE + SSV_PACKET_CHECKSUM_BYTES_SIZE);
		l_bufferBytesSize 		= SSV_PACKET_HEADER_BYTES_SIZE + SSV_PACKET_SERVO_ID_BYTES_SIZE + SSV_PACKET_DATA_LENGTH_BYTES_SIZE + SSV_PACKET_CMD_BYTES_SIZE + l_parametersBytesSize + SSV_PACKET_CHECKSUM_BYTES_SIZE;

	// Read on the device serial port
		l_parameters[0] = 0;
		writeDeviceSerialPort(p_servoId, p_cmd, l_parameters);
		l_readBytesNb = read(m_deviceSerialPort, l_buffer, l_bufferBytesSize);

	// Convert the result
		l_cmdLengthInBytes = getCmdLength(p_cmd);
		convertCmdParameters(p_cmd, l_cmdLengthInBytes, l_buffer, p_buffer);

	return 1;
}



uint32_t SSV::CLx16a::getCmdLength(uint32_t p_cmd)
{
	uint32_t 	l_cmdLengthInBytes;		//< Include : data length, command value, command parameters and checksum

	switch(p_cmd)
	{
		case SSV_SERVO_MESSAGE_MOVE_TIME_WRITE:
			l_cmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_WRITE:
			l_cmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_MOVE_START:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_MOVE_STOP:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_ID_WRITE:
			l_cmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_ID_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_ADJUST:
			l_cmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_WRITE:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_ANGLE_LIMIT_WRITE:
			l_cmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_ANGLE_LIMIT_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_VIN_LIMIT_WRITE:
			l_cmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_VIN_LIMIT_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_WRITE:
			l_cmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_TEMP_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_VIN_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_POS_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_OR_MOTOR_MODE_WRITE:
			l_cmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_OR_MOTOR_MODE_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_WRITE:
			l_cmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_LED_CTRL_WRITE:
			l_cmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_LED_CTRL_READ:
			l_cmdLengthInBytes = 3;
			break;

		case SSV_SERVO_MESSAGE_LED_ERROR_WRITE:
			l_cmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_LED_ERROR_READ:
			l_cmdLengthInBytes = 3;
			break;

		default:
			l_cmdLengthInBytes = 0;
			break;
	}

	return l_cmdLengthInBytes;
}



uint32_t SSV::CLx16a::getRetCmdLength(uint32_t p_cmd)
{
	uint32_t 	l_retCmdLengthInBytes;		//< Include : data length, command value, command parameters and checksum

	switch(p_cmd)
	{
		case SSV_SERVO_MESSAGE_MOVE_TIME_READ:
			l_retCmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_READ:
			l_retCmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_ID_READ:
			l_retCmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_READ:
			l_retCmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_ANGLE_LIMIT_READ:
			l_retCmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_VIN_LIMIT_READ:
			l_retCmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_READ:
			l_retCmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_TEMP_READ:
			l_retCmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_VIN_READ:
			l_retCmdLengthInBytes = 5;
			break;

		case SSV_SERVO_MESSAGE_POS_READ:
			l_retCmdLengthInBytes = 5;
			break;

		case SSV_SERVO_MESSAGE_OR_MOTOR_MODE_READ:
			l_retCmdLengthInBytes = 7;
			break;

		case SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_READ:
			l_retCmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_LED_CTRL_READ:
			l_retCmdLengthInBytes = 4;
			break;

		case SSV_SERVO_MESSAGE_LED_ERROR_READ:
			l_retCmdLengthInBytes = 4;
			break;

		default:
			l_retCmdLengthInBytes = 0;
			break;
	}

	return l_retCmdLengthInBytes;
}



int SSV::CLx16a::setCmdParameters(uint32_t p_cmd, unsigned char* p_buffer, double* p_parameter)
{
	switch(p_cmd)
	{
		case SSV_SERVO_MESSAGE_MOVE_TIME_WRITE:
			// AngleValue
				uint32_t l_angleValue;
				l_angleValue = (uint32_t) (p_parameter[0] / ((double) SSV_ANGLE_DEG_LIMIT_MAX) * ((double) SSV_ANGLE_LIMIT_MAX));
				if(l_angleValue < SSV_ANGLE_LIMIT_MIN)
				{
					l_angleValue = SSV_ANGLE_LIMIT_MIN;
				}
				else if(l_angleValue > SSV_ANGLE_LIMIT_MAX)
				{
					l_angleValue = SSV_ANGLE_LIMIT_MAX;
				}
				p_buffer[5] = l_angleValue & 0x00FF;		// Parameter 1 > Lower 8 bits of angle value
				p_buffer[6] = (l_angleValue & 0xFF00) >> 8;	// Parameter 2 > Higher 8 bits of angle value

			// Time Value
				p_buffer[7] = 0x00;							// Parameter 3 > Lower 8 bits of time value
				p_buffer[8] = 0x00;							// Parameter 4 > Higher 8 bits of time value
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_READ:
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_WRITE:
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_READ:
			break;

		case SSV_SERVO_MESSAGE_MOVE_START:
			break;

		case SSV_SERVO_MESSAGE_MOVE_STOP:
			break;

		case SSV_SERVO_MESSAGE_ID_WRITE:
			break;

		case SSV_SERVO_MESSAGE_ID_READ:
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_ADJUST:
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_WRITE:
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_READ:
			break;

		case SSV_SERVO_MESSAGE_ANGLE_LIMIT_WRITE:
			break;

		case SSV_SERVO_MESSAGE_ANGLE_LIMIT_READ:
			break;

		case SSV_SERVO_MESSAGE_VIN_LIMIT_WRITE:
			break;

		case SSV_SERVO_MESSAGE_VIN_LIMIT_READ:
			break;

		case SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_WRITE:
			break;

		case SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_READ:
			break;

		case SSV_SERVO_MESSAGE_TEMP_READ:
			break;

		case SSV_SERVO_MESSAGE_VIN_READ:
			break;

		case SSV_SERVO_MESSAGE_POS_READ:
			break;

		case SSV_SERVO_MESSAGE_OR_MOTOR_MODE_WRITE:
			// Servo mode
				p_buffer[5] = 0x01;										// Parameter 1 > Servo mode (0 : position control mode / 1 : motor control mode)

			// Null value
				p_buffer[6] = 0x00;										// Parameter 2 > Null value

			// Rotation speed value
				double 		l_rotationSpeedValue;
				uint32_t 	l_rotationSpeedValueTmp;
				l_rotationSpeedValue = p_parameter[0];
				if(l_rotationSpeedValue < SSV_ROTATION_SPEED_VALUE_LIMIT_MIN)
				{
					l_rotationSpeedValue = SSV_ROTATION_SPEED_VALUE_LIMIT_MIN;
				}
				else if(l_rotationSpeedValue > SSV_ROTATION_SPEED_VALUE_LIMIT_MAX)
				{
					l_rotationSpeedValue = SSV_ROTATION_SPEED_VALUE_LIMIT_MAX;
				}
				l_rotationSpeedValueTmp = (uint32_t) l_rotationSpeedValue;
				p_buffer[7] = l_rotationSpeedValueTmp & 0x00FF;			// Parameter 3 > Lower 8 bits of rotation speed value
				p_buffer[8] = (l_rotationSpeedValueTmp & 0xFF00) >> 8;	// Parameter 4 > Higher 8 bits of rotation speed value
			break;

		case SSV_SERVO_MESSAGE_OR_MOTOR_MODE_READ:
			break;

		case SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_WRITE:
			break;

		case SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_READ:
			break;

		case SSV_SERVO_MESSAGE_LED_CTRL_WRITE:
			break;

		case SSV_SERVO_MESSAGE_LED_CTRL_READ:
			break;

		case SSV_SERVO_MESSAGE_LED_ERROR_WRITE:
			break;

		case SSV_SERVO_MESSAGE_LED_ERROR_READ:
			break;

		default:
			break;
	}

	return 1;
}



uint32_t SSV::CLx16a::getCmdChecksum(unsigned char* p_buffer, uint32_t p_bufferBytesSize)
{
	uint32_t 		l_checksum;
	uint32_t 		l_checksumBracket;
	uint32_t 		l_index;

	l_checksumBracket = 0;
	for(l_index = 2; l_index < p_bufferBytesSize - 1; l_index++)
	{
		l_checksumBracket += p_buffer[l_index];
	}
	if(l_checksumBracket < 255)
	{
		l_checksum = ~l_checksumBracket;
	}
	else
	{
		l_checksum = ~(l_checksumBracket & 0x00FF);
	}
	return l_checksum;
}



uint32_t SSV::CLx16a::convertCmdParameters(uint32_t p_cmd, uint32_t p_cmdLengthInBytes, unsigned char* p_bufferToConvert, void* p_buffer)
{
	uint32_t 		l_offsetInBytes = SSV_PACKET_HEADER_BYTES_SIZE + SSV_PACKET_SERVO_ID_BYTES_SIZE + SSV_PACKET_DATA_LENGTH_BYTES_SIZE + SSV_PACKET_CMD_BYTES_SIZE;
	unsigned char 	l_parameter1;
	unsigned char 	l_parameter2;

	switch(p_cmd)
	{
		case SSV_SERVO_MESSAGE_MOVE_TIME_READ:
			break;

		case SSV_SERVO_MESSAGE_MOVE_TIME_WAIT_READ:
			break;

		case SSV_SERVO_MESSAGE_ID_READ:
			break;

		case SSV_SERVO_MESSAGE_ANGLE_OFFSET_READ:
			break;

		case SSV_SERVO_MESSAGE_ANGLE_LIMIT_READ:
			break;

		case SSV_SERVO_MESSAGE_VIN_LIMIT_READ:
			break;

		case SSV_SERVO_MESSAGE_TEMP_MAX_LIMIT_READ:
			break;

		case SSV_SERVO_MESSAGE_TEMP_READ:
			signed short l_temperature;
			l_parameter1	= p_bufferToConvert[l_offsetInBytes];
			l_temperature	= l_parameter1;
			memcpy(p_buffer, &l_temperature, sizeof(signed short));
			break;

		case SSV_SERVO_MESSAGE_VIN_READ:
			signed short l_voltage;
			l_parameter1 	= p_bufferToConvert[l_offsetInBytes];
			l_parameter2 	= p_bufferToConvert[l_offsetInBytes+1];
			l_voltage 		= (l_parameter1) + (l_parameter2 << 8);
			memcpy(p_buffer, &l_voltage, sizeof(signed short));
			break;

		case SSV_SERVO_MESSAGE_POS_READ:
			signed short l_position;
			l_parameter1 	= p_bufferToConvert[l_offsetInBytes];
			l_parameter2 	= p_bufferToConvert[l_offsetInBytes+1];
			l_position 		= (l_parameter1) + (l_parameter2 << 8);
			if(l_position < SSV_ANGLE_LIMIT_MIN)
			{
				l_position = SSV_ANGLE_LIMIT_MIN;
			}
			else if(l_position > SSV_ANGLE_LIMIT_MAX)
			{
				l_position = SSV_ANGLE_LIMIT_MAX;
			}
			l_position = (signed short) ((((double) l_position) / ((double) SSV_ANGLE_LIMIT_MAX)) * ((double) SSV_ANGLE_DEG_LIMIT_MAX));
			memcpy(p_buffer, &l_position, sizeof(signed short));
			break;

		case SSV_SERVO_MESSAGE_OR_MOTOR_MODE_READ:
			break;

		case SSV_SERVO_MESSAGE_LOAD_OR_UNLOAD_READ:
			break;

		case SSV_SERVO_MESSAGE_LED_CTRL_READ:
			break;

		case SSV_SERVO_MESSAGE_LED_ERROR_READ:
			break;

		default:
			break;
	}

	return 1;
}
