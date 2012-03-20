#ifndef STORE_H
#define STORE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void store_tests( void );

void readByte(void* adr);
void readWord(void* adr, uint16_t* data);
void writeWord(void* adr, uint16_t data);
void eeprom_read(void* data, void* addr, size_t num);
void eeprom_write(void* data, void* out, size_t num);

#ifdef __cplusplus
}
#endif
#endif

/** \file */
