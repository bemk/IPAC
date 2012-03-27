#ifndef PROCESSSTREAM_H
#define PROCESSSTREAM_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h>

#include <dev/debug.h>
#ifdef ETHERNUT2
#include <dev/lanc111.h>
#else
#include <dev/nicrtl.h>
#endif

#include <sys/version.h>
#include <sys/confnet.h>
#include <sys/heap.h>
#include <sys/bankmem.h>
#include <sys/thread.h>
#include <sys/timer.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <netinet/tcp.h>

#include <pro/dhcp.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Determine the compiler.
 */
#if defined(__IMAGECRAFT__)
#define CC_STRING   "ICCAVR"
#elif defined(__GNUC__)
#define CC_STRING   "AVRGCC"
#else
#define CC_STRING   "Compiler unknown"
#endif

/*!
 * \example mnut03/mnut03.c
 *
 * Medianut Tutorial - Part 3.
 */

/*! 
 * \brief Device for debug output. 
 */
#define DBG_DEVICE devDebug0

/*! 
 * \brief Device name for debug output. 
 */
#define DBG_DEVNAME "uart0"

/*! 
 * \brief Baudrate for debug output. 
 */
#define DBG_BAUDRATE 115200

/*! 
 * \brief Unique MAC address of the Ethernut Board. 
 */
#define MY_MAC { 0x00, 0x06, 0x98, 0x30, 0x02, 0x76 }

/*! 
 * \brief Unique IP address of the Ethernut Board. 
 *
 * Ignored if DHCP is used. 
 */
#define MY_IPADDR "192.168.192.100"

/*! 
 * \brief IP network mask of the Ethernut Board.
 *
 * Ignored if DHCP is used. 
 */
#define MY_IPMASK "255.255.255.0"

/*! 
 * \brief Gateway IP address for the Ethernut Board.
 *
 * Ignored if DHCP is used. 
 */
#define MY_IPGATE "192.168.192.1"

/*!
 * \brief IP address of the radio station.
 */
#define RADIO_IPADDR "91.121.117.196"
//#define RADIO_IPADDR "64.236.34.196"

/*!
 * \brief Port number of the radio station.
 */
#define RADIO_PORT 8000

/*!
 * \brief URL of the radio station.
 */
#define RADIO_URL "/stream"
//#define RADIO_URL "/stream/1020"
/*!
 * \brief Size of the header line buffer.
 */
#define MAX_HEADERLINE 512

/*!
 * \brief TCP buffer size.
 */
#define TCPIP_BUFSIZ 15000

/*!
 * \brief Maximum segment size. 
 *
 * Choose 536 up to 1460. Note, that segment sizes above 536 may result in 
 * fragmented packets. Remember, that Ethernut doesn't support TCP 
 * fragmentation.
 */
#define TCPIP_MSS 1460

/*
 * Socket receive timeout.
 */
#define TCPIP_READTIMEOUT 3000

 struct tekstScrollInfo{
        char *title;
        char *Description;
};

/**
* \fn PlayMp3Stream
* \brief feeds the mp3 decoder chip with data, resulting in a playing stream
* \return void
*/
void PlayMp3Stream(FILE *stream, u_long metaint);

/**
* \fn *ConnectStation
* \brief connects to the station using the given details for location
* \return a file stream filled with mp3 blocks
*/
FILE *ConnectStation(TCPSOCKET *sock, u_long ip, u_short port, u_long *metaint);

/**
* \fn reload_display
* \brief refreshes the text on the display
* \return void
*/
void reload_display(void);

/**
* \brief protected description containing artist+titel extracted from meta-inf
*/
extern volatile char Description[];

extern int volume;

#ifdef __cplusplus
}
#endif

#endif
