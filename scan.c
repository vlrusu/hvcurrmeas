/*
 * scan.c
 *      Prueba de I2C por software usando WiringPi
 *      Reinoso G. 09/02/2017
 *
 *      gcc -lwiringPi soft_i2c.c scan.c -o scan
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <mcp23x0817.h>
#include <mcp23s17.h>
#include <linux/spi/spidev.h>

#include "soft_i2c.h"


enum MCPs{MCPHV0, MCPHV1, MCPHV2, MCPHV3};


#define MCPPINBASE 2000
#define CALDACPINBASE 3000

#define SPISPEED 10000000

#define ADDRESS 0x68


int main (void)
{
	int addr;
	int pin;

	int samplerate;
	int gain;
	int config;
	int realChan;

	uint8_t firstbyte;
	uint8_t secondbyte;
	uint8_t thirdbyte;

	int fullscale = 2.048;
	float val;
	float current;
	int ADS112C04_FS = 65536;
	float ADS112C04_REFV = 2.048;
	
	samplerate = 2;
	gain = 0;
	realChan = 0;
	
	
	
	puts ("I2C scan for soft_i2c project");


	wiringPiSetup () ;
	wiringPiSPISetup (0, SPISPEED);
	wiringPiSPISetupMode (1, SPISPEED, SPI_MODE_2 ); //clock for this should be inverted
	wiringPiSPISetup (2, SPISPEED);


	//setup MCPs
	for (int imcp = MCPHV0; imcp<=MCPHV3; imcp++){
	  mcp23s17Setup (MCPPINBASE+16*imcp, 2, 0x20 + imcp );
	}


	/* pin = MCPPINBASE+16*MCPHV2+1; */
	/* pinMode(pin, OUTPUT); */
	/* digitalWrite(pin, HIGH); */
	
	//	i2c_t i2c = i2c_init(MCPPINBASE+16*MCPHV3+1,MCPPINBASE+16*MCPHV2+3);

	i2c_t i2c = i2c_init(MCPPINBASE+16*MCPHV3+7,MCPPINBASE+16*MCPHV2+1);
	//i2c_t i2c = i2c_init(9,8);

	/* for (addr = 0; addr < 128; addr++) { */
	/* 	i2c_start(i2c); */
	/* 	if (i2c_send_byte(i2c, addr << 1 | I2C_READ) == I2C_ACK) */
	/* 		printf (" * Device found at %0xh\n", addr); */
	/* 	i2c_stop(i2c); */
	/* } */

	i2c_start(i2c);
	/* if (i2c_send_byte(i2c, ADDRESS << 1 | I2C_WRITE) != I2C_ACK) { */
	/* 	fprintf(stderr, "No device found at address %0xh\n", ADDRESS); */
	/* 	exit(2); */
	/* } */
	
	config = 0x80 | (realChan << 5) | (samplerate << 2) | (gain) ;
	printf (" Config %0xh\n", config);
	
	if (i2c_send_byte(i2c, ADDRESS << 1 | I2C_WRITE) != I2C_ACK) fprintf(stderr, "No device found at address %0xh\n", ADDRESS);
	if (i2c_send_byte(i2c, config) != I2C_ACK) fprintf(stderr, "No device found at address %0xh\n", ADDRESS); 

	i2c_stop(i2c);


	i2c_start(i2c);
	if (i2c_send_byte(i2c, ADDRESS << 1 | I2C_READ) != I2C_ACK) fprintf(stderr, "No device found at address %0xh\n", ADDRESS);
	firstbyte = i2c_read_byte(i2c);
	i2c_send_bit(i2c, I2C_ACK);
	secondbyte = i2c_read_byte(i2c);
	i2c_send_bit(i2c, I2C_ACK);
	thirdbyte = i2c_read_byte(i2c);
	i2c_send_bit(i2c, I2C_NACK);

	
	
	printf (" Firstbyte %0xh\n", firstbyte);
	printf (" Secondbyte %0xh\n", secondbyte);
	printf (" Thirdbyte %0xh\n", thirdbyte);	
	i2c_stop(i2c);



	val = firstbyte*256. + secondbyte;

	if (val > ADS112C04_FS-1)
	  val = -ADS112C04_REFV+(val-ADS112C04_FS-1)*ADS112C04_REFV/ADS112C04_FS;
        else
	  val = ADS112C04_REFV*val/ADS112C04_FS;
	

	printf( "val=%4.5f\n",val);

	current = val*1e2;
	      
	printf( "current (nA)=%4.5f\n",current);

	
	return 0;
}
