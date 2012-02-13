#include "application.h"
#include <sys/mutex.h>

int btn_pushed(void);
int app_kbd_start(void);


/**
 * \fn key_handle
 * \brief The thread to scan for key events and handle them when occuring
 * \param arg
 * \brief Pointer to the (unused) arguments
 */
THREAD(key_handle, arg)
{
        NutThreadSetPriority(10);
        int i = 0;
        for (;;)
        {
                LcdBackLight(LCD_BACKLIGHT_OFF);
                if (i > 0)
                {
                        LcdBackLight(LCD_BACKLIGHT_ON);
                        i--;
                }
                NutSleep(250);
                int key_code = KbGetKey();
                if (key_code > 0 && key_code < 12)
                        i = 40;
                switch(key_code)
                {
                case KEY_01:
                        LcdChar('1');
                        break;
                case KEY_02:
                        LcdChar('2');
                        break;
                case KEY_03:
                        LcdChar('3');
                        break;
                case KEY_04:
                        LcdChar('4');
                        break;
                case KEY_05:
                        LcdChar('5');
                        break;
                case KEY_ALT:
                        LcdChar('A');
                        break;
                case KEY_ESC:
                        LcdChar('E');
                        break;
                case KEY_UP:
                        LcdChar('U');
                        break;
                case KEY_OK:
                        LcdChar('O');
                        break;
                case KEY_LEFT:
                        LcdChar('L');
                        break;
                case KEY_DOWN:
                        LcdChar('D');
                        break;
                case KEY_RIGHT:
                        LcdChar('R');
                        break;
                default:
                        break;
                }
        }
}

/**
 * \fn app_kbd_start
 * \brief This starts the thread which handles the keyboard events
 */
int app_kbd_start()
{
        NutThreadCreate("kbd_monitor", key_handle, NULL, 512);
        return 0;
}