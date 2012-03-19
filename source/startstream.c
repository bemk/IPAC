 /*! startstream.c is supposed to initialize a working shoutcaststream
 *  
 *
 */

 
#define LOG_MODULE  LOG_MAIN_MODULE

/*--------------------------------------------------------------------------*/
/*  Include files                                                           */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/version.h>
#include <dev/irqreg.h>

#include "system.h"
#include "portio.h"
#include "display.h"
#include "remcon.h"
#include "keyboard.h"
#include "led.h"
#include "log.h"
#include "uart0driver.h"
#include "mmc.h"
#include "watchdog.h"
#include "flash.h"
#include "spidrv.h"
#include "processStream.h"
#include "vs10xx.h"
#include <time.h>
#include "rtc.h"
#include "wan.h"
#include "startstream.h"
#include "menu.h"

TCPSOCKET* SocketCreate(TCPSOCKET *sock,u_short mss,u_long rx_to,u_short tcpbufsiz);
int is_running = 0;
int *volume;
/* contains the args for mp3streamthread*/
typedef struct {
        char *name;
        FILE *stream;
        u_long metaint;
        int sleeptime;
}thread_args;


/* Stuff for the internet connection*/
TCPSOCKET *sock;
FILE *stream;
u_long radio_ip;
u_short tcpbufsiz = TCPIP_BUFSIZ;
u_long rx_to = TCPIP_READTIMEOUT;
u_short mss = TCPIP_MSS;
u_long metaint;

THREAD(PlayMp3StreamThread, arg)
{
        //TCPSOCKET *sock;
	u_short mss = 1460;
	u_long rx_to = 3000;
	u_short tcpbufsiz = 8000;
	FILE *stream;
	sock = SocketCreate(sock,mss,rx_to,tcpbufsiz);
	
	/*
        * Connect the radio station.
        */
        u_long radio_ip = inet_addr(RADIO_IPADDR);
        stream = ConnectStation(sock, radio_ip, RADIO_PORT, &metaint);
	
	
	if(stream)
	{
		PlayMp3Stream(stream,metaint);
		fclose(stream);
        }
        
        NutTcpCloseSocket(sock);
        NutThreadExit();
        for (;;);
}

TCPSOCKET* SocketCreate(TCPSOCKET *sock,u_short mss,u_long rx_to,u_short tcpbufsiz)
{
        if ((sock = NutTcpCreateSocket()) == 0)
        {
                puts("Error: Can't create socket");
               for(;;);
        }

        /* 
        * Set socket options. Failures are ignored.
        */
        if (NutTcpSetSockOpt(sock, TCP_MAXSEG, &mss, sizeof(mss)))
                printf("Sockopt MSS failed\n");
        if (NutTcpSetSockOpt(sock, SO_RCVTIMEO, &rx_to, sizeof(rx_to)))
                printf("Sockopt TO failed\n");
        if (NutTcpSetSockOpt(sock, SO_RCVBUF, &tcpbufsiz, sizeof(tcpbufsiz)))
                printf("Sockopt rxbuf failed\n");
        return sock;
}
	
int start_playing()
{
        if(is_running == 0 )
        {		
				
		is_running = 1;
		playing = 1;
                
                NutThreadCreate("mp3Stream" , PlayMp3StreamThread, NULL, 256);
             
	}
	else if(is_running == 1)
	{

		printf("already running + %s", Description );
		return 0;
	
	}
        return 0;
}

void stop_stream(struct menu* this)
{      
        is_running = 0;
        playing = 0;   
}
