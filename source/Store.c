#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include <dev/uartavr.h>
#include <dev/uart.h>
#include <dev/twif.h>
//#include <dev/at24c.h>
#include <dev/eeprom.h>

#include <sys/device.h>

#include "system.h"
#include "uart0driver.h"
#include "log.h"

#define LOG_MODULE  LOG_MAIN_MODULE


void test(void)
{
	EEInit();
    
       // LogMsg_P(LOG_INFO, PSTR(""),read);
}



void readData(uint16_t addres)
{
        
}

void writeData(uint16_t addres, void *data)
{
        
}





