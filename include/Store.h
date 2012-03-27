#ifndef STORE_H
#define STORE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
<<<<<<< HEAD
=======
/**
 * \struct eeprom_data 
 * \brief Standard data structure for permanent data storage
 */
struct eeprom_data{
        /**
         * \var volume
         * \brief store volume 
         * \var stream_port
         * \brief port for streaming 
         * \var stream_path
         * \brief stream url 
         * \var stream_ip[16]
         * \brief stream ip adress 
         * \var db_server
         * \brief database server 
         * \var db_name_size
         * \brief size of the database name
         * \var alarm_type
         * \brief
         */
        uint8_t volume;
        uint16_t stream_port;
        char stream_path[128];
        char stream_ip[16];
        char db_server[64];
        size_t db_name_size;
        uint8_t alarm_type;
        uint8_t alarm_type2;
};
>>>>>>> 011b8cc... Committing test for working volume written to EEPROM. Volume is saved and restored.

void store_tests( void );

void readByte(void* adr);
void readWord(void* adr, uint16_t* data);
void writeWord(void* adr, uint16_t data);
void eeprom_read(void* data, void* addr, size_t num);
void eeprom_write(void* data, void* out, size_t num);
<<<<<<< HEAD

=======
void store_settings(void);
void restore_settings(void);

extern struct eeprom_data eeprom;
>>>>>>> 011b8cc... Committing test for working volume written to EEPROM. Volume is saved and restored.
#ifdef __cplusplus
}
#endif
#endif

/** \file */
