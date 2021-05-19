#include <stdint.h>
#include <stdio.h>

#include "mcp3426.h"
#include "soft_i2c.h"

#include <wiringPi.h>

void _mcp3426_init(mcp3426_t* self, int scl, int sda){

  self->i2c = i2c_init(scl,sda);
  self->config = 0x80;


}

void _mcp3426_setconfig(mcp3426_t* self, int gain , int samplerate  , int mode , int channel ){

  i2c_start(self->i2c);
  self->config = 0x80 | (channel << 5) | (mode << 4) | (samplerate << 2) | (gain) ;
  //  printf (" Config %0xh\n", config);
	
  if (i2c_send_byte(self->i2c, MCP3426_ADDRESS << 1 | I2C_WRITE) != I2C_ACK) fprintf(stderr, "No device found at address %0xh\n", MCP3426_ADDRESS);
  if (i2c_send_byte(self->i2c, self->config) != I2C_ACK) fprintf(stderr, "No device found at address %0xh\n", MCP3426_ADDRESS); 
  
  i2c_stop(self->i2c);
  
}


float _mcp3426_read(mcp3426_t* self){

  float val;
  uint8_t firstbyte;
  uint8_t secondbyte;
  uint8_t thirdbyte;


  while (1){
    i2c_start(self->i2c);
    if (i2c_send_byte(self->i2c, MCP3426_ADDRESS << 1 | I2C_READ) != I2C_ACK) fprintf(stderr, "No device found at address %0xh\n", MCP3426_ADDRESS);
    firstbyte = i2c_read_byte(self->i2c);
    i2c_send_bit(self->i2c, I2C_ACK);
    secondbyte = i2c_read_byte(self->i2c);
    i2c_send_bit(self->i2c, I2C_ACK);
    thirdbyte = i2c_read_byte(self->i2c);
    i2c_send_bit(self->i2c, I2C_NACK);

    if ( (thirdbyte & 0x80)== 0) break;
    delayMicroseconds(1000);
    
    if ( (thirdbyte & 0x7F) != (self->config & 0x7F) ) fprintf(stderr, "Bad address in config %0xh should be %0xh\n", thirdbyte, self->config);

	
	
  /* printf (" Firstbyte %0xh\n", firstbyte); */
  /* printf (" Secondbyte %0xh\n", secondbyte); */
  /* printf (" Thirdbyte %0xh\n", thirdbyte); */
    i2c_stop(self->i2c);
  }



  val = firstbyte*256. + secondbyte;

  if (val > MCP3426_FS-1)
    val = -MCP3426_REFV+(val-MCP3426_FS-1)*MCP3426_REFV/MCP3426_FS;
  else
    val = MCP3426_REFV*val/MCP3426_FS;

  
  return val;
  

}
