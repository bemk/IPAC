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
        char* tst = "XXXXX";
        //writeWord((void*)46, 0x6948);
        //readWord((void*)46, (void*)tst);
        eeprom_write("blaat", (void*)46, 5);
        eeprom_read(tst, (void*)46, 5);
        printf("\nRead %s\n", tst);
}

void readByte(void* adr)
{
        uint8_t ByteOfData;
        ByteOfData = eeprom_read_byte((uint8_t*)adr);
        printf("\n%c\n", ByteOfData);  
        
}
/**
*Reading word from eeprom.
*/
void readWord(void* adr, uint16_t* data)
{
        *data = eeprom_read_word(adr);
}
/**
*Write word to eeprom.
*/
void writeWord(void* adr, uint16_t data)
{
        eeprom_write_word(adr, data);
}

void readBlock(void)
{
        uint8_t StringOfData[10];
        eeprom_read_block((void*)&StringOfData, (const void*)46, 10);
        printf("\n Words are %s\n", StringOfData);
}

void eeprom_read(void* data, void* addr, size_t num)
{
        int i = 0;
        for (; i < num; i++, addr++)
        {
                ((char*)data)[i] = eeprom_read_byte(addr);
        }
}

void eeprom_write(void* data, void* out, size_t num)
{
        int i = 0;
        for (; i < num; i++, out++)
        {
                eeprom_write_byte(out, ((char*)data)[i]);
        }
}




