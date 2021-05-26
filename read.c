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
#include "mcp3426.h"


enum MCPs{MCPHV0, MCPHV1, MCPHV2, MCPHV3};


#define MCPPINBASE 2000
#define CALDACPINBASE 3000

#define SPISPEED 10000000


int main (void)
{
	float val;
	float current;
	
	mcp3426_t adc[48];

	wiringPiSetup () ;
	wiringPiSPISetup (2, SPISPEED);


	//setup MCPs
	for (int imcp = MCPHV0; imcp<=MCPHV3; imcp++){
	  mcp23s17Setup (MCPPINBASE+16*imcp, 2, 0x20 + imcp );
	}


	//	_mcp3426_init(&adc[0], MCPPINBASE+16*MCPHV3+7,MCPPINBASE+16*MCPHV2+1);
	//_mcp3426_init(&adc[0], MCPPINBASE+16*MCPHV0+2,MCPPINBASE+16*MCPHV0+1);
	//	_mcp3426_init(&adc[1], MCPPINBASE+16*MCPHV3+8,MCPPINBASE+16*MCPHV2+1);
	//_mcp3426_init(&adc[1], MCPPINBASE+16*MCPHV0+3,MCPPINBASE+16*MCPHV0+1);

	for (int i=0; i<14; i++){
	  _mcp3426_init(&adc[i], MCPPINBASE+16*MCPHV0+(i+2),MCPPINBASE+16*MCPHV0+1);
	}
	for (int i=14; i<24; i++){
	  _mcp3426_init(&adc[i], MCPPINBASE+16*MCPHV1+(i-14),MCPPINBASE+16*MCPHV0+1);	
	}
	for (int i=24; i<38; i++){
	  _mcp3426_init(&adc[i], MCPPINBASE+16*MCPHV2+(i-24+2),MCPPINBASE+16*MCPHV2+1);	
	}
	for (int i=38; i<48; i++){
	  _mcp3426_init(&adc[i], MCPPINBASE+16*MCPHV3+(i-38),MCPPINBASE+16*MCPHV2+1);	
	}

	for (uint8_t i= 0; i< 48; i++){ 
	//void _mcp3426_setconfig(mcp3426_t* self, int gain, int samplerate, int mode, int channel);
	  printf( "z%d\n",i );

	  _mcp3426_setconfig(&adc[i],0,2,0,0);
	  val = _mcp3426_read(&adc[i]);
	  printf( "val=%4.5f\n",val);
	  current = val*1e2;
	  printf( "current (nA)=%4.5f\n",current);


	  _mcp3426_setconfig(&adc[i],0,2,0,1);
	  val = _mcp3426_read(&adc[i]);
	  printf( "val=%4.5f\n",val);
	  current = val*1e2;
	  printf( "current (nA)=%4.5f\n",current);
	  printf( "\n" );
	}




	
	return 0;
}
