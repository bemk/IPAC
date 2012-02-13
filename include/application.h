#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <stdio.h>
#include <string.h>

#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/version.h>
#include <dev/irqreg.h>
#include <time.h>

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
#include "menu.h"
#include "rtc.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct interupt {
        void (*handle)(void);
        bool waiting;
};

int app_kbd_start(void);

#ifdef __cplusplus
}
#endif

#endif
