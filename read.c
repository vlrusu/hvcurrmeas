
 
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
	
	mcp3426_t adc;

	wiringPiSetup () ;
	wiringPiSPISetup (2, SPISPEED);


	//setup MCPs
	for (int imcp = MCPHV0; imcp<=MCPHV3; imcp++){
	  mcp23s17Setup (MCPPINBASE+16*imcp, 2, 0x20 + imcp );
	}


	_mcp3426_init(&adc, MCPPINBASE+16*MCPHV3+7,MCPPINBASE+16*MCPHV2+1);

	//void _mcp3426_setconfig(mcp3426_t* self, int gain, int samplerate, int mode, int channel);
	_mcp3426_setconfig(&adc,0,2,0,0);
	val = _mcp3426_read(&adc);

	printf( "val=%4.5f\n",val);
	current = val*1e2;
	printf( "current (nA)=%4.5f\n",current);


	_mcp3426_setconfig(&adc,0,2,0,1);
	val = _mcp3426_read(&adc);
	printf( "val=%4.5f\n",val);
	current = val*1e2;
	printf( "current (nA)=%4.5f\n",current);
	




	
	return 0;
}
