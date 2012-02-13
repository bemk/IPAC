#include "application.h"
#include <sys/mutex.h>

int btn_pushed(void);
int app_kbd_start(void);

MUTEX key_lock;
MUTEX light_lock;
struct interupt* key_table[12];

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
 * \fn app_kbd_update_keys
 * \brief Is used to import a new set of function pointers to the table
 * \param new_map
 * \brief The pointer to the new table, gets copied so keep track!
 */
int app_kbd_update_keys(struct interupt** new_map)
{
        NutMutexLock(&key_lock);

        int i = 0;

        for (; i < 12; i++)
        {
                key_table[i] = new_map[i];
        }

        NutMutexUnlock(&key_lock);
        return 0;
}

/**
 * \fn app_kbd_start
 * \brief This starts the thread which handles the keyboard events
 */
int app_kbd_start()
{
        NutMutexInit(&key_lock);
        NutMutexInit(&light_lock);
        NutThreadCreate("kbd_monitor", key_handle, NULL, 512);
        return 0;
}