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



void store_tests(void)
{
        writeWord(46,0x6948);
        readWord(46);
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
        char str[] = {0,0,0};
        uint16_t wordOfData;
        wordOfData = eeprom_read_word((uint16_t*)adr);
        str[0] = (wordOfData & 0xFF);
        str[1] = ((wordOfData >> 8) & 0xFF);
        printf("\nThe Words is %s\n", str);
     
}
/**
*Write word to eeprom.
*/
void writeWord(uint16_t *adr, uint16_t data)
{
        eeprom_write_word(adr,data);
}

void readBlock(void)
{
        uint8_t StringOfData[10];
        eeprom_read_block((void*)&StringOfData, (const void*)46, 10); 
        printf("\n Words are %c\n", StringOfData);
}





