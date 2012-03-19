#ifndef _STARTSTREAM_H
#define _STARTSTREAM_H

#include "menu.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \var playing
 * \brief simple integer that keeps a check if mp3 stream is playing or not.
 */
int playing;

/**
 * \fn stop_stream 
 * \brief closes the socket, and kills the mp3play thread. this makes everything ready for a new stream.
 * \return void
 */ 
void stop_stream(struct menu* this);

/** 
 * \fn start playing
 * \brief play the stream
 * \return error code
 */  
int start_playing(void);

#ifdef __cplusplus
}
#endif

#endif
