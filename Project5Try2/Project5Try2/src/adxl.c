/*
 * adxl.c
 */ 
#include "adxl.h"
#include "i2cmaster.h"

// I2C protocol for writing to a register on the accelerometer
void writeReg(unsigned char address, unsigned char val) {
	// Set write mode
	i2c_rep_start((0x1d << 1) | I2C_WRITE);
	// set the address
	i2c_write(address);
	// write the value
	i2c_write(val);
	// finish
	i2c_stop();
}

// I2C protocol for reading a register from the accelerometer
unsigned char readReg(unsigned char address) {
	// set write mode to set the address
	i2c_start_wait((0x1d << 1) | I2C_WRITE);
	i2c_write(address);
	
	// set to read mode to read the data
	i2c_rep_start((0x1d << 1) | I2C_READ);
	unsigned char res = i2c_readNak();
	
	// finish
	i2c_stop();
	
	return res;
}

// I2C protocol for reading two consecutive registers (16 bits) from the accelerometer
// Data on the accelerometer is stored in little endian
int readReg16(unsigned char address) {
	int res = 0;
	
	// set write mode to set the address for the first register
	i2c_start_wait((0x1d << 1) | I2C_WRITE);
	i2c_write(address);
	
	// set to read mode to read the data (8LSB)
	i2c_rep_start((0x1d << 1) | I2C_READ);
	res = i2c_readNak();
	
	// set write mode to set the address for the next register
	i2c_start_wait((0x1d << 1) | I2C_WRITE);
	i2c_write(address + 1);
	
	// set to read mode to read the data (8MSB)
	i2c_rep_start((0x1d << 1) | I2C_READ);
	res |= ((int) i2c_readNak()) << 8;
	
	// Finish
	i2c_stop();
	
	return res;
}

// The output of the accelerometer is 13 bits, but is still signed.
// Since its stored in 16 bits, we need to worry about the correct value.
// Then, scale it to be between -1 and 1
float formatRaw(int raw) {
	int val = raw;
	// Check if the sign bit is 1
	if (val >> 13 != 0) {
		// value should be negative, mask the lower 13 bits and
		// then take the twos complement to get the correct negative number
		val = -1 * ((0b1111111111111 & ~val) + 1);
		} else {
		// value is positive, just mask the lower 13 bits
		val = 0b1111111111111 & val;
	}
	// scale to be between -1 and 1
	return val * .0039f;
}

// Initializes the adxl345 chip
void initAdxl() {
	// Initialize the i2c communication
	i2c_init();
	
	// Set the power mode
	writeReg(0x2d, 0x08);
	
	// Set the format
	unsigned char format = readReg(0x31);
	format &= ~(0x0f);
	format |= 0b1011;
	writeReg(0x31, format);
	
	// Disable the fifo cache
	writeReg(0x38, 0);
}

// Read x,y, and z accelerometer values into the first three
// indices of the data
void readAdxl(float* data) {
	data[0] = formatRaw(readReg16(0x32));
	data[1] = formatRaw(readReg16(0x34));
	data[2] = formatRaw(readReg16(0x36));
}