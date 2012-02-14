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
                if (key_code > 0 && key_code < 20)
                        i = 40;
                        if (mnu == NULL)
                                continue;
                switch(key_code)
                {
                case KEY_01:
                        if (mnu->btn1 == NULL)
                                continue;
                        mnu->btn1(mnu);
                        break;
                case KEY_02:
                        if (mnu->btn2 == NULL)
                                continue;
                        mnu->btn2(mnu);
                        break;
                case KEY_03:
                        if (mnu->btn3 == NULL)
                                continue;
                        mnu->btn3(mnu);
                        break;
                case KEY_04:
                        if (mnu->btn4 == NULL)
                                continue;
                        mnu->btn4(mnu);
                        break;
                case KEY_05:
                        if (mnu->btn5 == NULL)
                                continue;
                        mnu->btn5(mnu);
                        break;
                case KEY_ALT:
                        if (mnu->btn_alt == NULL)
                                continue;
                        mnu->btn_alt(mnu);
                        break;
                case KEY_ESC:
                        if (mnu->btn_esc == NULL)
                                continue;
                        mnu->btn_esc(mnu);
                        break;
                case KEY_UP:
                        if (mnu->btn_up == NULL)
                                continue;
                        mnu->btn_up(mnu);
                        break;
                case KEY_OK:
                        if (mnu->btn_ok == NULL)
                                continue;
                        mnu->btn_ok(mnu);
                        break;
                case KEY_LEFT:
                        if (mnu->btn_left == NULL)
                                continue;
                        mnu->btn_left(mnu);
                        break;
                case KEY_DOWN:
                        if (mnu->btn_down == NULL)
                                continue;
                        mnu->btn_down(mnu);
                        break;
                case KEY_RIGHT:
                        if (mnu->btn_right == NULL)
                                continue;
                        mnu->btn_right(mnu);
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