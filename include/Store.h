#ifndef STORE_H
#define STORE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void store_tests( void );

void readByte(uint8_t *adr);
void readWord(uint16_t adr);
void writeWord(uint16_t *adr, char data);

#ifdef __cplusplus
}
#endif
#endif

/** \file */
