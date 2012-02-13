#include "application.h"
#include "menu.h"
#include <sys/heap.h>

char* itoc[24] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13",
				  "14","15","16","17","18","19","20","21","22","23"};

void mem_set(void* dst, int c, int size)
{
	char* tmp = dst;
	for (; size > 0; size--, tmp = (void*)((unsigned int)tmp++))
		tmp = (c & 0xFF);
}

struct menu *mnu = NULL;

THREAD(mnu_thread, args)
{
	NutThreadSetPriority(10);
	for (;;)
	{
		NutSleep(100);
		if (mnu == NULL)
			continue;
		reset();
		LcdWriteLine1(mnu->top_line);
		LcdWriteLine2(mnu->messages[mnu->message_id]);
	}
}

/**
 * \fn std_btn_up
 * \brief Standard move up in the current menu
 */
void std_btn_up(struct menu* this)
{
        this->message_id++;
        if(this->message_id >= this->no_messages)
                this->message_id = 0;
}

/**
 * \fn std_btn_down
 * \brief Standard move down in the current menu
 */
void std_btn_down(struct menu* this)
{
        this->message_id--;
        if(this->message_id < 0)
                this->message_id = this->no_messages-1;
}

/**
 * \fn std_btn_left
 * \brief Standard move to the parent menu
 */
void std_btn_left(struct menu* this)
{
		if (this->parent_ctor != NULL)
			this->parent_ctor();
}

void std_btn_right(struct menu* this)
{
		switch(mnu->message_id)
		{
			case 0:
				tz_menu_init();
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				break;
			default:
				break;
		}
}

void std_mnu_init()
{
	if (mnu == NULL)
		mnu = malloc(sizeof(*mnu));
	if (mnu == NULL)
		return -1;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Menu";
	mnu->messages[0] = "Time zones";
	mnu->messages[1] = "Hello world!";
	mnu->no_messages = 2;
	mnu->message_id = 0;
	
	mnu->btn_up = std_btn_up;
	mnu->btn_down = std_btn_down;
	mnu->btn_right = std_btn_right;
	
	NutThreadCreate("Menu", mnu_thread, NULL, 512);
}

void tz_menu_init()
{
	if (mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Time zone";
	mnu->messages[0] = "UTC + %";
	mnu->parent_ctor = std_mnu_init;
	mnu->btn_left = std_btn_left;
}
