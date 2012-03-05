

#include <stdio.h>

extern void test( void );
void EEInit(void);
int read( uint16_t addr, void *data, uint16_t len );
int write( uint16_t addr, void *data, uint16_t len );
void run( void );