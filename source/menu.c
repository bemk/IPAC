#include "application.h"
#include "menu.h"
#include <sys/heap.h>

static void tz_menu_init();
static void klok_menu_init();
static void entertainment_menu_init();
static void alarm_menu_init();
static void stream_menu_init();
static void sd_menu_init();
static void set_alarm_menu();
static void play_menu_init();

char* itoc[24] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13",
                        "14","15","16","17","18","19","20","21","22","23"};

struct menu *mnu = NULL;
bool msg_updated = TRUE;

THREAD(mnu_thread, args)
{
	NutThreadSetPriority(10);
	for (;;)
	{
		NutSleep(100);
		if (mnu == NULL)
			continue;
                if (msg_updated)
                {
                        LcdWriteLine1(mnu->top_line);
                        LcdWriteLine2(mnu->messages[mnu->message_id]);
                        msg_updated = FALSE;
                }
	}
}

/**
 * \fn tz_btn_up
 * \brief What will happen when we push the button up in the timezone menu
 */
static void tz_btn_up(struct menu* this)
{
        this->mnu_cnt++;
        if (this->mnu_cnt >= 24)
                this->mnu_cnt = 0;
        msg_updated = TRUE;
}

/**
 * \fn tz_btn_down
 * \brief
 */
static void tz_btn_down(struct menu* this)
{
        this->mnu_cnt--;
        if  (this->mnu_cnt < 0)
             this->mnu_cnt = 23;
        msg_updated = TRUE;
}

/**
 * \fn std_btn_up
 * \brief Standard move up in the current menu
 */
static void std_btn_up(struct menu* this)
{
        this->message_id++;
        if(this->message_id >= this->no_messages)
                this->message_id = 0;
        msg_updated = TRUE;
}

/**
 * \fn std_btn_down
 * \brief Standard move down in the current menu
 */
static void std_btn_down(struct menu* this)
{
        this->message_id--;
        if(this->message_id < 0)
                this->message_id = this->no_messages-1;
        msg_updated = TRUE;
}

/**
 * \fn std_btn_left
 * \brief Standard move to the parent menu
 */
static void std_btn_left(struct menu* this)
{
        if (this->parent_ctor != NULL)
                this->parent_ctor();
                msg_updated = TRUE;
}

static void std_btn_right(struct menu* this)
{
	switch(mnu->message_id)
	{
	case 0:
		tz_menu_init();
		break;
	case 1:
		klok_menu_init();
		break;
	case 2:
		entertainment_menu_init();
		break;
	case 3:
		alarm_menu_init();
		break;
        case 4:
                stream_menu_init();
                break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		break;
	default:
		break;
        }
        msg_updated = TRUE;
}

static void std_entertainment_btn_right(struct menu* this)
{
	switch(mnu->message_id)
	{
	case 0:
		stream_menu_init();
		break;
	case 1:
                sd_menu_init();
                break;
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
	msg_updated = TRUE;
}

static void alarm_btn_right(struct menu* this)
{
        switch(mnu->message_id)
        {
        case 0:
                set_alarm_menu();
                break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
                break;
        default:
                break;
        }
        msg_updated = TRUE;
}
static void std_play_btn_right(struct menu* this)
{
	switch(mnu->message_id)
	{
	case 0:
                play_menu_init();
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

static void std_mnu_build()
{
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Menu";
	mnu->messages[0] = "Time zones";
	mnu->messages[1] = "Klok instellen";
	mnu->messages[2] = "Entertainment";
	mnu->messages[3] = "Alarm";
	mnu->no_messages = 4;
	mnu->message_id = 0;

	mnu->btn_up = std_btn_up;
	mnu->btn_down = std_btn_down;
	mnu->btn_right = std_btn_right;
}

void std_mnu_init()
{
	if (mnu == NULL)
		mnu = malloc(sizeof(*mnu));
	if (mnu == NULL)
		return -1;
	std_mnu_build();
	NutThreadCreate("Menu", mnu_thread, NULL, 512);
}

static void tz_menu_init()
{
	if (mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Time zone";
	mnu->messages[0] = "UTC + %";
	mnu->parent_ctor = std_mnu_build;
        mnu->btn_left = std_btn_left;
        mnu->btn_down = tz_btn_down;
        mnu->btn_up = tz_btn_up;
}

static void klok_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Klok";
	mnu->messages[0] = "De tijd";
	mnu->parent_ctor = std_mnu_build;
	mnu->btn_left = std_btn_left;
}

void entertainment_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Entertainment";
	mnu->messages[0] = "Internet Radio";
	mnu->messages[1] = "SD music";
	mnu->no_messages = 2 ;

	mnu->parent_ctor = std_mnu_build;
	mnu->btn_left = std_btn_left;
	mnu->btn_up = std_btn_up;
	mnu->btn_down = std_btn_down;
        mnu->btn_right = std_entertainment_btn_right;
}

void alarm_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu,0, sizeof(struct menu));
	mnu->top_line = "Alarm";
	mnu->messages[0] = "Set alarm";
	mnu->messages[1] = "Set ON/OFF";
	mnu->no_messages = 2 ;

	mnu->parent_ctor = std_mnu_build;
	mnu->btn_left = std_btn_left;
	mnu->btn_up = std_btn_up;
	mnu->btn_down = std_btn_down;
        mnu->btn_right = alarm_btn_right;
}

static void stream_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Internet Radio";
	mnu->messages[0] = "Stream";
	mnu->parent_ctor = entertainment_menu_init;
	mnu->btn_left = std_btn_left;
        mnu->btn_right = std_play_btn_right;
        
}

static void sd_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "SD music";
	mnu->messages[0] = "bestand kiezen";
	mnu->parent_ctor = entertainment_menu_init;
	mnu->btn_left = std_btn_left;
        mnu->btn_right = std_play_btn_right;
        
}

static void set_alarm_menu()
{
        if(mnu == NULL)
                return;
        memset(mnu,0, sizeof(struct menu));
        mnu->top_line = "Set Alarm";
        mnu->messages[0] = "HH:MM";
        mnu->no_messages = 1 ;
        mnu->parent_ctor = alarm_menu_init;
        mnu->btn_left = std_btn_left;
}
static void play_menu_init()
{
        if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Music is playing";
	mnu->messages[0] = "Artist - Title";
	mnu->parent_ctor = entertainment_menu_init;
	mnu->btn_left = std_btn_left;
}
