/*
 * Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

/*!
 * $Log$
 */

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

#include <vs10xx.h>

#include "display.h"

#include <pro/dhcp.h>
#include "testStream.h"
#include "wan.h"

typedef struct {
    char *title;
	char *description;
} tekstScrollInfo;

/*
 * Description String
 */
 char Description[40];
 //void DisplayInfo(char title[], char description[]);
/*
 * Title String
 */

 char Title[40];



/*!
 * \brief Connect to a radio station.
 *
 * \param sock TCP socket for this connection.
 * \param ip   IP address of the server to connect.
 * \param port Port number of the server to connect.
 *
 * \return Stream pointer of the established connection on success.
 *         Otherwise 0 is returned.
 */
FILE *ConnectStation(TCPSOCKET *sock, u_long ip, u_short port, u_long *metaint)
{
    int rc;
    FILE *stream;
    u_char *line;
    u_char *cp;

    /* 
     * Connect the TCP server. 
     */
    printf("Connecting %s:%u...", inet_ntoa(ip), port);
    if ((rc = NutTcpConnect(sock, ip, port))) {
		int i;
		i = NutTcpError(sock);
        printf("Error: Connect failed with %d\n", i);
		if(i == 10060)
		{
			printf("ERROR: connection already exist!");
		}
        return 0;
    }
    puts("OK");

    if ((stream = _fdopen((int) sock, "r+b")) == 0) {
        puts("Error: Can't create stream");
        return 0;
    }

    /*
     * Send the HTTP request.
     */
    printf("GET %s HTTP/1.0\n\n", RADIO_URL);
    fprintf(stream, "GET %s HTTP/1.0\r\n", RADIO_URL);
    fprintf(stream, "Host: %s\r\n", inet_ntoa(ip));
    fprintf(stream, "User-Agent: Ethernut\r\n");
    fprintf(stream, "Accept: */*\r\n");
    fprintf(stream, "Icy-MetaData: 1\r\n");
    fprintf(stream, "Connection: close\r\n");
    fputs("\r\n", stream);
    fflush(stream);

    /*
     * Receive the HTTP header.
     */
    line = malloc(MAX_HEADERLINE);
    while(fgets(line, MAX_HEADERLINE, stream)) {

        /*
         * Chop off the carriage return at the end of the line. If none
         * was found, then this line was probably too large for our buffer.
         */
        cp = strchr(line, '\r');
        if(cp == 0) {
            puts("Warning: Input buffer overflow");
            continue;
        }
        *cp = 0;

        /*
         * The header is terminated by an empty line.
         */
        if(*line == 0) {
            break;
        }
        if(strncmp(line, "icy-metaint:", 12) == 0) {
            *metaint = atol(line + 12);
        }
		if(strncmp( line, "icy-description:",16) == 0)
		{
			int i;
			int j = 0;
			for(i =16; i<strlen(line); i++)
			{
				Description[j] = line[i];
				j++;
			}
		}
		putchar('\n');
		if(strncmp( line, "icy-name:",9) == 0)
		{
			int i;
			int j = 0;
			for(i =9; i<strlen(line); i++)
			{
				Title[j] = line[i];
				j++;
			}
		}
        printf("%s\n", line);
    }
    putchar('\n');

    free(line);
	//DisplayInfo(Title,Description);

    return stream;
}

void DisplayInfo(char title[], char description[])
{
//	start_display();
//	LcdBackLight(1);
//	LcdWriteLine1(title);
//	LcdWriteLine2(description);
	
}


/*
 * Process embedded meta data.
 */
int ProcessMetaData(FILE *stream)
{
    u_char blks = 0;
    u_short cnt;
    int got;
    int rc = 0;
    u_char *mbuf;

    /*
     * Wait for the lenght byte.
     */
    got = fread(&blks, 1, 1, stream);
    if(got != 1) {
        return -1;
    }
    if (blks) {
        if (blks > 32) {
            printf("Error: Metadata too large, %u blocks\n", blks);
            return -1;
        }

        cnt = blks * 16;
        if ((mbuf = malloc(cnt + 1)) == 0) {
            return -1;
        }

        /*
         * Receive the metadata block.
         */
        for (;;) {
            if ((got = fread(mbuf + rc, 1, cnt, stream)) <= 0) {
                return -1;
            }
            if ((cnt -= got) == 0) {
                break;
            }
            rc += got;
            mbuf[rc] = 0;
        }

        printf("\nMeta='%s'\n", mbuf);
		

		
		
		
		LcdWriteLine2(mbuf);
        free(mbuf);
    }
	
    return 0;
}

/*
 * \brief Play MP3 stream.
 *
 * \param stream Socket stream to read MP3 data from.
 */
void PlayMp3Stream(FILE *stream, u_long metaint)
{
printf("PLAAAY");
    size_t rbytes;
    u_char *mp3buf;
    u_char ief;
    int got = 0;
    u_long last;
    u_long mp3left = metaint;

    /*
     * Initialize the MP3 buffer. The NutSegBuf routines provide a global
     * system buffer, which works with banked and non-banked systems.
     */
    if (NutSegBufInit(8192) == 0) {
        puts("Error: MP3 buffer init failed");
        return;
    }

    /* 
     * Initialize the MP3 decoder hardware.
     */
    if (VsPlayerInit() || VsPlayerReset(0)) {
        puts("Error: MP3 hardware init failed");
        return;
    }
    VsSetVolume(0, 0);

    /* 
     * Reset the MP3 buffer. 
     */
    ief = VsPlayerInterrupts(0);
    NutSegBufReset();
    VsPlayerInterrupts(ief);
    last = NutGetSeconds();

    for (;;) {
        /*
         * Query number of byte available in MP3 buffer.
         */
        ief = VsPlayerInterrupts(0);
        mp3buf = NutSegBufWriteRequest(&rbytes);
        VsPlayerInterrupts(ief);

        /*
         * If the player is not running, kick it.
         */
        if (VsGetStatus() != VS_STATUS_RUNNING) {
            if(rbytes < 1024 || NutGetSeconds() - last > 4UL) {
                last = NutGetSeconds();
                puts("Kick player");
                VsPlayerKick();
            }
        }

        /* 
         * Do not read pass metadata. 
         */
        if (metaint && rbytes > mp3left) {
            rbytes = mp3left;
        }

        /* 
         * Read data directly into the MP3 buffer. 
         */
        while (rbytes) {
            if ((got = fread(mp3buf, 1, rbytes, stream)) > 0) {
                ief = VsPlayerInterrupts(0);
                mp3buf = NutSegBufWriteCommit(got);
                VsPlayerInterrupts(ief);

                if (metaint) {
                    mp3left -= got;
                    if (mp3left == 0) {
                        ProcessMetaData(stream);
                        mp3left = metaint;
                    }
                }

                if(got < rbytes && got < 512) {
                    printf("%lu buffered\n", NutSegBufUsed());
                    NutSleep(250);
                }
                else {
                    NutThreadYield();
                }
            } else {
                break;
            }
            rbytes -= got;
        }

        if(got <= 0) {
            break;
        }
    }
}

