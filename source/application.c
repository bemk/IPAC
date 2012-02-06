#include "application.h"
#include <sys/mutex.h>

int btn_pushed(void);
int app_kbd_start(void);

MUTEX key_lock;
MUTEX light_lock;
struct interupt* key_table[12];

THREAD(light_backlight, arg)
{
        if (NutMutexTrylock(&light_lock) != 0)
        {
                NutThreadExit();
                for (;;);
        }

        LcdBackLight(LCD_BACKLIGHT_ON);
        NutSleep(10000);
        LcdBackLight(LCD_BACKLIGHT_OFF);

        NutMutexUnlock(&light_lock);

        NutThreadExit();
        for(;;); // Keep compiler from complaining
}

THREAD(key_handle, arg)
{
        for (;;)
        {
                if (KbWaitForKeyEvent(1000) == KB_ERROR)
                        continue;

                int key_code = KbGetKey();
                NutMutexLock(&key_lock);
                if (key_table[key_code] != NULL)
                {
                        btn_pushed();
                        key_table[key_code]->handle();
                }
                NutMutexUnlock(&key_lock);
        }
}

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

int app_kbd_start()
{
        NutMutexInit(&key_lock);
        NutMutexInit(&light_lock);
        NutThreadCreate("kbd_monitor", key_handle, NULL, 512);
        return 0;
}

int btn_pushed()
{
        NutThreadCreate("bkg_light", light_backlight, NULL, 256);
        return 0;
}
