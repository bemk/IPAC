#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include <dev/uartavr.h>
#include <dev/uart.h>
#include <dev/twif.h>
#include <dev/board.h>

#include <sys/heap>
#include <sys/device.h>

#include "system.h"
#include "uart0driver.h"
#include "eeprom.h"
#include "log.h"

static struct at24c at24c32s;
const struct at24c myeeprom ={32, 128, 32*128, I2C_SLA_AT24C,2,20};

u_long baud = 0; 

void test(void)
{
	NutRegisterDevice(&DEV_DEBUG,0,0);
	freeopen(DEV_DEBUG_NAME, "w",stdout);
	_ioc1(_fileno(stdout), UART_SETSPEED, &baud);
	
	LogMsg_P(LOG_INFO, PSTR("******EEPROM TEST *****\n"));
	//printf( "\n*** EEPROM Test ***\n");
	
	baud = 100000;
	TwInit(0);
	TwIOCt1(TWI_SETSPEED, &baud);
	EEInit();
        run();
}



void EEInit(void)
{
	at24c32s.PageSize = 32;
	at24c32s.NumOfPage = 128;             
        at24c32s.EepromSize = 32*128;         
        //at24c32s.SlaveAddress = I2C_SLA_AT24C;
        at24c32s.IAddrW = 2;                  
        at24c32s.Timeout = 20;   
}

void run(void)
{
        /* Clear the Receive buffer */
    memset( rxBuffer, 0x00, sizeof( rxBuffer));
    memset( txBuffer, 0xff, sizeof( txBuffer));
 
    printf( "Read\n" );
    read( 2, rxBuffer, 64);
    HexDump( rxBuffer, 64 );
 
    if( !strncmp( rxBuffer, teststr, strlen( teststr)))
        printf( "Test successfull, data is equal!\n");
    else
        printf( "Test failed, data not equal!\n");
 
    printf( "\nInit: Fill 0xFF\n" );
    write( 0, txBuffer, 64);
    read( 0, rxBuffer, 64);
    HexDump( rxBuffer, 64 );
    
}


int read( uint16_t addr, void *data, uint16_t len )
{
	return At24cWrite( &at24c32s, (uint8_t *)data, len, addr );
}

int write( uint16_t addr, void *data, uint16_t len)
{
        return At24cWrite( &at24c32s, (uint8_t *)data, len, addr ); 
}