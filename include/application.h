#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <stdio.h>
#include <string.h>

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

#include <time.h>
#include "rtc.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct interupt {
        void (*handle)(void);
        bool waiting;
};

struct menu_item {
        void (*btn0)(void);
        void (*btn1)(void);
        void (*btn2)(void);
        void (*btn3)(void);
        void (*btn4)(void);
        void (*btn5)(void);
        void (*btn_ok)(void);
        void (*btn_esc)(void);
        void (*btn_up)(void);
        void (*btn_down)(void);
        void (*btn_left)(void);
        void (*btn_right)(void);
        void (*btn_alt)(void);

        char* top_line;
        char** messages;
};

int app_kbd_start(void);

#ifdef __cplusplus
}
#endif

#endif