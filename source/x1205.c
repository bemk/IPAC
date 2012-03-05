#define LOG_MODULE  LOG_X1205_MODULE

#include <cfg/os.h>
#include <dev/twif.h>
#include <sys/event.h>

#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "x1205.h"

// x1205 slave address, see source code (rtc.*) streamit
#define I2C_SLA_RTC 0x6F


/* ******************************************************************* */
void x1205Init(void)
/*
 * Initialiseer TWI, gebruik NUT/OS functionaliteit.
 * see: http://www.ethernut.de/api/ of http://www.ethernut.de/nutwiki/I2C
 *
 */
{
	TwInit(0);
	// 0 betekent dat wij master zijn, alle andere devices op de bus
	// zijn slaves.

	// Optioneel:
	//TwIOCtr(TWI_SETSPEED, &....);

	 LogMsg_P(LOG_INFO, PSTR("x1205Init()"));
}

//
char x1205ReadByte(unsigned char addr)
{
	int retval = 1;
	u_char writeBuffer[2];
	u_char readBuffer[2];
	writeBuffer[0] = 0;
	writeBuffer[1] = addr;  // SC register in x1205 RTC, datasheet page 10

	// TwMasterTransact schrijft eerst en leest daarna. Dit is wat we willen.
	// Schrijf eerst werk register we willen lezen, daarna lees het register

	if( TwMasterTransact(I2C_SLA_RTC, writeBuffer, 2, readBuffer, 2, NUT_WAIT_INFINITE) != 2 )
	{
		retval = -1;
		LogMsg_P(LOG_INFO, PSTR("error x1205readByte()"));
	}
	else
	{
               		LogMsg_P(LOG_INFO, PSTR("before: [%02x]"), readBuffer[0]);


                       // sprintf(readBuffer[0], "%d", readBuffer[0]);
                       // LogMsg_P(LOG_INFO, PSTR("after: [%02d]"), readBuffer[0]);
	}

	return readBuffer[0];
}

//
int x1205Enable(void)
{
	u_char writeBuffer[5];

	writeBuffer[0] = 0;
	writeBuffer[1] = 0x3f;
	writeBuffer[2] = 0x02;
	TwMasterTransact(I2C_SLA_RTC, writeBuffer, 3, 0, 0, NUT_WAIT_INFINITE);
	writeBuffer[2] = 0x06;
	TwMasterTransact(I2C_SLA_RTC, writeBuffer, 3, 0, 0, NUT_WAIT_INFINITE);

	return 1;
}

//
int x1205WriteByte(unsigned char addr, unsigned char data ) //, unsigned char cnt)
{
	int retval = 0;

	u_char writeBuffer[3];

	writeBuffer[0] = 0;
	writeBuffer[1] = addr;
	writeBuffer[2] = data;
	TwMasterTransact(I2C_SLA_RTC, writeBuffer, 3, 0, 0, NUT_WAIT_INFINITE);

	return retval;
}

