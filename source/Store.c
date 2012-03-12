#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include <avr/eeprom.h>

#include <sys/device.h>

#include "system.h"
#include "uart0driver.h"
#include "log.h"

#define LOG_MODULE  LOG_MAIN_MODULE

unsigned char *write = "hello";
unsigned char *buff;
u_int numbers;

void store_tests(void)
{
        //readWords(90);
        numbers = OnChipNvMemLoad(46,buff,32);
        if(numbers == 0)
        {
                printf("Yes");
                
        }
        else
        {
                printf("NO");
        }
        
}

void readByte(uint8_t *adr)
{
        uint8_t ByteOfData;
        ByteOfData = eeprom_read_byte((uint8_t*)adr);
        LogMsg_P(LOG_INFO, PSTR("Byte ===%d"),ByteOfData);        
}

void readWords(uint16_t *adr)
{
        uint16_t WordOfData;
        WordOfData = eeprom_read_word((uint16_t*)adr);
        printf("\n%X\n", WordOfData);
}

void writeWords(uint16_t *adr, char data)
{
     //   uint16_t WordOfData;
        eeprom_write_word(adr,data);
        
}

void readBlock()
{
        uint8_t StringOfData[10];
        eeprom_read_block((void*)&StringOfData, (const void*)12, 10); 
        printf("\nSucces\n%X\n", StringOfData);
}





