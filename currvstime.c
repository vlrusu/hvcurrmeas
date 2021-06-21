#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <mcp23x0817.h>
#include <mcp23s17.h>
#include <linux/spi/spidev.h>
#include <time.h>

#include "soft_i2c.h"
#include "mcp3426.h"


enum MCPs{MCPHV0, MCPHV1, MCPHV2, MCPHV3};


#define MCPPINBASE 2000
#define CALDACPINBASE 3000

#define SPISPEED 8000000


int main (void)
{
	float val;

	double duration = 3600; //duration of measurement time, in seconds
	//	double duration = 60; //duration of measurement time, in seconds
	
	FILE *fpt;
	fpt = fopen("currvstime.csv", "w+"); //output file
	
	//	clock_t tic = clock();
	time_t tic = time(NULL);
	time_t toc = time(NULL);
	//	clock_t toc = clock(); //define clock ticks
	
	//48 total measurements are possible
	mcp3426_t adc[48];
	float current[48];
	
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
	int count = 0;
	while((double)(toc - tic) < duration){
	  //get current vs time


	  for (int idac = 0; idac<48; idac++){


	    /* if (idac==0 || idac==3 ||idac==5 || idac== 7 || idac == 9 || idac == 11 || idac ==13 || idac==15 || */
	    /* 	idac==17 || idac==19 ||idac==22 || idac== 24 || idac == 26 || idac == 28 || idac ==30 || idac==32 || */
	    /* 	idac==34 || idac==36 ||idac==38 || idac== 40 || idac == 42 || idac == 44 || idac ==46  */

	    /* 	) */

	
	    if (idac==0 || idac==2 || idac==4 || idac== 6 || idac == 8 || idac == 10 || idac ==12  || idac==14||
	    	idac==16 || idac==18 ||idac==20 || idac== 22 || idac == 24 || idac == 25 || idac ==28 || idac==30 ||
	    	idac==32 || idac==34 ||idac==36 || idac== 38 || idac == 40 || idac == 42 || idac ==44 || idac == 46

	    	){
	
	      printf( "---z%d---\n",count );
	      _mcp3426_setconfig(&adc[idac],0,2,0,0);
	      val = _mcp3426_read(&adc[idac]);
	      printf( "val 1 = %4.5f\n",val);
	      current[idac] = val*1e2;
	      printf( "current 1 (nA) = %4.5f\n",current[idac]);
	      _mcp3426_setconfig(&adc[idac],0,2,0,1);
	      val = _mcp3426_read(&adc[idac]);
	      printf( "val 2 = %4.5f \n",val);
	      current[idac+1] = val*1e2;
	      printf( "current 2 (nA) = %4.5f\n",current[idac+1]);
	      printf( "\n" );
	      count++;
	    }
	  }
	  /*

	  printf( "---z%d---\n",9 );
	  _mcp3426_setconfig(&adc[9],0,2,0,0);
	  val = _mcp3426_read(&adc[9]);
	  printf( "val 1 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 1 (nA) = %4.5f\n",current);
	  _mcp3426_setconfig(&adc[9],0,2,0,1);
	  val = _mcp3426_read(&adc[9]);
	  printf( "val 2 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 2 (nA) = %4.5f\n",current);
	  printf( "\n" );
	  
	  printf( "---z%d---\n",18 );
	  _mcp3426_setconfig(&adc[18],0,2,0,0);
	  val = _mcp3426_read(&adc[18]);
	  printf( "val 1 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 1 (nA) = %4.5f\n",current);
	  _mcp3426_setconfig(&adc[18],0,2,0,1);
	  val = _mcp3426_read(&adc[18]);
	  printf( "val 2 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 2 (nA) = %4.5f\n",current);
	  printf( "\n" );
	  
	  printf( "---z%d---\n",30 );
	  _mcp3426_setconfig(&adc[30],0,2,0,0);
	  val = _mcp3426_read(&adc[30]);
	  printf( "val 1 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 1 (nA) = %4.5f\n",current);
	  _mcp3426_setconfig(&adc[30],0,2,0,1);
	  val = _mcp3426_read(&adc[30]);
	  printf( "val 2 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 2 (nA) = %4.5f\n",current);
	  printf( "\n" );
	  
	  printf( "---z%d---\n",40 );
	  _mcp3426_setconfig(&adc[40],0,2,0,0);
	  val = _mcp3426_read(&adc[40]);
	  printf( "val 1 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 1 (nA) = %4.5f\n",current);
	  _mcp3426_setconfig(&adc[40],0,2,0,1);
	  val = _mcp3426_read(&adc[40]);
	  printf( "val 2 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 2 (nA) = %4.5f\n",current);
	  printf( "\n" );
	  */
	  //	  toc = clock();
	  toc = time(NULL);
	  //	  fprintf(fpt, "%lf, \t %lf \t %lf\t %lf \t %lf \t %lf\t %lf \t %lf \n", ((double)(toc - tic) / CLOCKS_PER_SEC), current[1], current[9], current[18], current[24], current[30], current[35], current [40], current[43]);
	  fprintf(fpt, "%lf, ", ((double)(toc - tic)));
	  for (int idac=0; idac<48; idac++){

	    if (idac==0 || idac==2 || idac==4 || idac== 6 || idac == 8 || idac == 10 || idac ==12  || idac==14||
	    	idac==16 || idac==18 ||idac==20 || idac== 22 || idac == 24 || idac == 25 || idac ==28 || idac==30 ||
	    	idac==32 || idac==34 ||idac==36 || idac== 38 || idac == 40 || idac == 42 || idac ==44 || idac == 46

	    	){




	    	/*     if (idac==0 ||idac==5 || idac== 7 || idac == 9 || idac == 11 || idac ==13 || */
		/* idac==17 || idac==19 ||idac==22 || idac== 24 || idac == 26 || idac == 28 || idac ==30 || idac==32 || */
		/* idac==34 || idac==36 ||idac==38 || idac== 40 || idac == 42 || idac == 44 || idac ==45  */

		/* ){ */

	      
	      fprintf(fpt, "%lf, ",current[idac]);
	      fprintf(fpt, "%lf, ",current[idac+1]); 
	    }
	  }
	  fprintf(fpt, "\n"); 
		
	  printf("Elapsed time (s): %lf\n", (double)(toc - tic) );
		
	}
	

	//read and print measurements
	/*for (uint8_t i= 0; i< 48; i++){ 
	//void _mcp3426_setconfig(mcp3426_t* self, int gain, int samplerate, int mode, int channel);
	  printf( "---z%d---\n",i );

	  _mcp3426_setconfig(&adc[i],0,2,0,0);
	  val = _mcp3426_read(&adc[i]);
	  printf( "val 1 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 1 (nA) = %4.5f\n",current);


	  _mcp3426_setconfig(&adc[i],0,2,0,1);
	  val = _mcp3426_read(&adc[i]);
	  printf( "val 2 = %4.5f\n",val);
	  current = val*1e2;
	  printf( "current 2 (nA) = %4.5f\n",current);
	  printf( "\n" );
	}*/



	fclose(fpt);
	
	return 0;
}
