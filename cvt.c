#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <mcp23x0817.h>
#include <mcp23s17.h>
#include <linux/spi/spidev.h>
#include <time.h>
#include <string.h>

#include "soft_i2c.h"
#include "mcp3426.h"

/* This code measures current vs time for the duration of 'timelimit'. The code will take data from adcs listed one per line in "input.txt". timelimit is entered as a command line argument, or the default timelimit given below will be used. */

enum MCPs{MCPHV0, MCPHV1, MCPHV2, MCPHV3};


#define MCPPINBASE 2000
#define CALDACPINBASE 3000

#define SPISPEED 10000000

#define MAX_LINE_LENGTH 100


int main (int argc, char* argv[])
{

	float val1, val2;
	float current1, current2;
	
	double timelimit = 0.1; //default measurement duration, in seconds
	
    	if(argc>=2){ //check for timelimit entered as command line argument
		char *ptr;
		timelimit = strtod(argv[1], &ptr); //convert string to double
	}
	
	char *path;
	char line[MAX_LINE_LENGTH] = {0};
    
	path = "input.txt";
	
	FILE *fpt;
	fpt = fopen("currvstime.csv", "w+"); //output file
	
	clock_t tic = clock();
	clock_t toc = clock(); //define clock ticks
	
	//48 total measurements are possible
	mcp3426_t adc[48];

	wiringPiSetup () ;
	wiringPiSPISetup (2, SPISPEED);


	//setup MCPs
	for (int imcp = MCPHV0; imcp<=MCPHV3; imcp++){
	  mcp23s17Setup (MCPPINBASE+16*imcp, 2, 0x20 + imcp );
	}

	//initialize
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
	
	while(((double)(toc - tic) / CLOCKS_PER_SEC) < timelimit){ //while measurements are being taken, print each line in the output
	    
		toc = clock();
	    
		fprintf(fpt, "%lf,", ((double)(toc - tic) / CLOCKS_PER_SEC)); //each line gets a time measurement
		
		//Get each line of input 
		FILE *file = fopen(path, "r");
	    
		while(fgets(line, MAX_LINE_LENGTH, file)){
			//get current vs time
			printf( "---z%d---\n", atoi(line) ); //atoi converts string to int
			_mcp3426_setconfig(&adc[atoi(line)],0,2,0,0);
			val1 = _mcp3426_read(&adc[atoi(line)]);
			printf( "val 1 = %4.5f\n",val1);
			current1 = val1*1e2;
			printf( "current 1 (nA) = %4.5f\n",current1);
	      
			_mcp3426_setconfig(&adc[atoi(line)],0,2,0,1);
			val2 = _mcp3426_read(&adc[atoi(line)]);
			printf( "val 2 = %4.5f \n",val2);
			current2 = val2*1e2;
			printf( "current 2 (nA) = %4.5f\n",current2);
			printf( "\n" );
	
			fprintf(fpt, " %lf, %lf,", current1, current2);
		}
		
		fclose(file);
		
		printf("Elapsed time (s): %lf\n\n", (double)(toc - tic) / CLOCKS_PER_SEC);
		
		fprintf(fpt, "\n"); //start new line
	}

	fclose(fpt);
	
	return 0;
}
