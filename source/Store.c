#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include <avr/eeprom.h>

#include <sys/device.h>

#include "system.h"
#include "uart0driver.h"
#include "log.h"
#include "Store.h"

#define LOG_MODULE  LOG_MAIN_MODULE

unsigned char data;
u_int numbers = 46;


void store_tests(void)
{
        writeWord(numbers, 'ha');
        readWord(numbers);      
}

void readByte(uint8_t *adr)
{
        uint8_t ByteOfData;
        ByteOfData = eeprom_read_byte((uint8_t*)adr);
        printf("\n%c\n", ByteOfData);       
}
/**
*Reading word from eeprom.
*/
void readWord(uint16_t adr)
{
        uint16_t wordOfData;
        wordOfData = eeprom_read_word((uint16_t*)adr);
        for(int i = 0; i <= sizeof(wordOfData);i++)
        {
                
        }
        printf("\nThe Words is %d\n", sizeof(wordOfData));
}
/**
*Write word to eeprom.
*/
void writeWord(uint16_t *adr, char data)
{
        eeprom_write_word(adr,data);
        
}

void readBlock(void)
{
        uint8_t StringOfData[10];
        eeprom_read_block((void*)&StringOfData, (const void*)12, 10); 
        printf("\nSucces\n%s\n", StringOfData);
}





