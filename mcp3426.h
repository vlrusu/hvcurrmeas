#ifndef _MCP3426_SOFT_H
#define _MCP3426_SOFT_H

#include <stdint.h>

#include "soft_i2c.h"

#define MCP3426_FS 65536
#define MCP3426_REFV 2.048
#define MCP3426_ADDRESS 0x68

typedef struct {
  int scl;
  int sda;
  int config;
  i2c_t i2c;
  
} mcp3426_t;


void _mcp3426_init(mcp3426_t* self, int scl, int sda);
void _mcp3426_setconfig(mcp3426_t* self, int gain, int samplerate, int mode, int channel);
float _mcp3426_read(mcp3426_t* self);

#endif
