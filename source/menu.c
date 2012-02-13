#include "application.h"
#include "menu.h"
#include <sys/heap.h>
#include <string.h>

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
			this->parent_ctor(this);
}

void std_btn_right(struct menu* this)
{
}

int std_mnu_init()
{
	mnu = malloc(sizeof(struct menu));
	if (mnu == NULL)
		return -1;
	//memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Menu";
	mnu->messages[0] = "text";
	mnu->messages[1] = "Hello world!";
	mnu->no_messages = 2;
	mnu->message_id = 0;
	
	mnu->btn_up = std_btn_up;
	mnu->btn_down = std_btn_down;
	
	NutThreadCreate("Menu", mnu_thread, NULL, 512);
	return 0;
}
