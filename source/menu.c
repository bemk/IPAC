#include "application.h"
#include "menu.h"
#include <sys/heap.h>
#include "rtc.h"

static void tz_btn_up(struct menu* this);
static void tz_btn_down(struct menu* this);
static void std_btn_up(struct menu* this);
static void std_btn_down(struct menu* this);
static void std_btn_left(struct menu* this);
static void std_btn_right(struct menu* this);
static void std_mnu_buttons(struct menu* this);

static void main_mnu_build(void);
static void tz_menu_init(void);
static void clock_menu_init(void);

static void entertainment_menu_init(void);
static void entertainment_btn_right(struct menu* this);
static void stream_menu_init(void);
static void sd_menu_init(void);
static void nav_to_preset_child(struct menu* this);
static void play_menu_init(void);

static void alarm_menu_init(void);
static void main_alarm_right(struct menu* this);
static void set_alarm_time_menu(void);
static void select_alarm_type(struct menu* this);
static void alarm_on_off_menu(void);
static void alarm_type_menu_init(void);
static void save_alarm_type(struct menu* this);
static void save_menu_init(void);

static void time_btn_up(struct menu* this);
static void time_btn_down(struct menu* this);
static void time_btn_left(struct menu* this);
static void time_btn_right(struct menu* this);
static void time_btn_ok(struct menu* this);

static void goose_menu_init(void);

char* itoc[24] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13",
                        "14","15","16","17","18","19","20","21","22","23"};

struct menu *mnu = NULL;
bool msg_updated = TRUE;
char* clock_msg = "XX:YY:ZZ";

THREAD(mnu_thread, args)
{
	NutThreadSetPriority(10);
	for (;;)
	{
		NutSleep(THREAD_SLEEP_TIME);
		if (mnu == NULL)
			continue;
                if (mnu->clock_set)
                {
                        LcdWriteLine1(mnu->top_line);
                        sprintf(clock_msg, "%02d:%02d:%02d", mnu->hVal, mnu->mVal, mnu->sVal);
                        LcdWriteLine2(clock_msg);
                        continue;
                }
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
 * \brief What will happen when we push the button up in the timezone menu.
 */
static void tz_btn_up(struct menu* this)
{
        this->mnu_cnt++;
        this->mnu_cnt %= 24;
        msg_updated = TRUE;
}

/**
 * \fn tz_btn_down
 * \brief What will happen we we push the button down in the timezone menu.
 */
static void tz_btn_down(struct menu* this)
{
        this->mnu_cnt--;
        if (this->mnu_cnt < 0)
                this->mnu_cnt = 23;
        msg_updated = TRUE;
}

/**
 * \fn std_btn_esc
 * \brief What will happen by default on esc press.
 */
static void std_btn_esc(struct menu* this)
{
        main_mnu_build();
        msg_updated = TRUE;
        return;
}

/**
 * \fn std_btn_up
 * \brief Standard move up in the current menu.
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
 * \brief Standard move down in the current menu.
 */
static void std_btn_down(struct menu* this)
{
        this->message_id--;
        if(this->message_id < 0)
                this->message_id = this->no_messages - 1;
        msg_updated = TRUE;
}

/**
 * \fn std_btn_left
 * \brief Standard move to the parent menu.
 */
static void std_btn_left(struct menu* this)
{
        if (this->parent_ctor != NULL)
                this->parent_ctor();
        msg_updated = TRUE;
}

/**
 * \fn std_btn_right
 * \brief Standard move to the child menu.
 */
static void std_btn_right(struct menu* this)
{
	switch(mnu->message_id)
	{
	case 0:
		tz_menu_init();
		break;
	case 1:
		clock_menu_init();
		break;
	case 2:
		entertainment_menu_init();
		break;
	case 3:
		alarm_menu_init();
		break;
        case 4:
                goose_menu_init();
                break;
	case 5:
	case 6:
	case 7:
	default:
		break;
        }
        msg_updated = TRUE;
}

/**
 * \fn std_mnu_buttons
 * \brief Sets standard button functions.
 */
static void std_mnu_buttons(struct menu* this)
{
        this->btn_up = std_btn_up;
        this->btn_down = std_btn_down;
        this->btn_left = std_btn_left;
        this->btn_esc = std_btn_esc;
}

/**
 * \fn main_mnu_init
 * \brief Initializes the application by starting up the main menu and the menu thread.
 */
void main_mnu_init()
{
	if (mnu == NULL)
		mnu = malloc(sizeof(*mnu));
	if (mnu == NULL)
		return;
	main_mnu_build();
	NutThreadCreate("Menu", mnu_thread, NULL, 1024);
}

/**
 * \fn main_mnu_build
 * \brief Builds the main menu.
 */
static void main_mnu_build()
{
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Menu";
	mnu->messages[0] = "Timezone";
	mnu->messages[1] = "Clock";
	mnu->messages[2] = "Entertainment";
	mnu->messages[3] = "Alarm";
        mnu->messages[4] = "Goose";
	mnu->no_messages = 5;
	mnu->message_id = 0;
	std_mnu_buttons(mnu);
        mnu->btn_right = std_btn_right;
}

/**
 * \fn tz_menu_init
 * \brief Inizializes the time zone menu.
 */
static void tz_menu_init()
{
        if (mnu == NULL)
                return;
        memset(mnu, 0, sizeof(struct menu));
        mnu->top_line = "Timezone";
        mnu->messages[0] = "UTC + %i";
        mnu->no_messages = 1;
        std_mnu_buttons(mnu);
        mnu->parent_ctor = main_mnu_build;
        mnu->btn_down = tz_btn_down;
        mnu->btn_up = tz_btn_up;
}

/**
 * \fn clock_mnu_init
 * \brief The timeclock menu.
 */
static void clock_menu_init()
{
        if(mnu == NULL)
                return;
        memset(mnu, 0, sizeof(struct menu));
        mnu->top_line = "Clock";
        std_mnu_buttons(mnu);
        mnu->parent_ctor = main_mnu_build;
        mnu->clock_set = TRUE;
        mnu->btn_up = time_btn_up;
        mnu->btn_down = time_btn_down;
        mnu->btn_left = time_btn_left;
        mnu->btn_right = time_btn_right;
        mnu->btn_ok = time_btn_ok;
}

/**
 * \fn time_btn_up
 * \brief raises the hours, minutes or seconds
 */
static void time_btn_up(struct menu* this)
{
        switch(this->time_field)
	{
	case 0:
		this->hVal++;
                this->hVal %= 24;
		break;
	case 1:
		this->mVal++;
                this->mVal %= 60;
		break;
	case 2:
                this->sVal++;
                this->sVal %= 60;
                break;
	default:
		break;
        }
        msg_updated = TRUE;
}

/**
 * \fn time_btn_down
 * \brief decrease the hours, minutes or seconds
 */
static void time_btn_down(struct menu* this)
{
        switch(this->time_field)
	{
	case 0:
		this->hVal--;
                if(this->hVal < 0)
                        this->hVal = 23;
		break;
	case 1:
		this->mVal--;
                if(this->mVal < 0)
                        this->mVal = 59;
		break;
	case 2:
                this->sVal--;
                if(this->sVal < 0)
                        this->sVal = 59;
                break;
	default:
		break;
        }
        msg_updated = TRUE;
}

/**
 * \fn time_btn_left
 * \brief navigate between hours, minutes and seconds
 */
static void time_btn_left(struct menu* this)
{
        this->time_field --;
        if (this->time_field < 0)
                this->time_field = 2;

        msg_updated = TRUE;
}

/**
 * \fn time_btn_right
 * \brief navigate between hours, minutes and seconds
 */
static void time_btn_right(struct menu* this)
{ 
        this->time_field ++;
        if (this->time_field > 2)
                this->time_field = 0;

        msg_updated = TRUE;
}

/**
 * \fn time_btn_ok
 * \brief save the hours, minutes and seconds
 */
static void time_btn_ok(struct menu* this)
{
        x1205WriteByte(0x30, BIN2BCD(this->sVal));
        x1205WriteByte(0x31, BIN2BCD(this->mVal));
        x1205WriteByte(0x32, BIN2BCD(this->hVal) | 0x80);
        
        msg_updated = TRUE;
}

/**
 * \fn entertainment_menu_init
 * \brief The entertainment selection menu which goes to play mode.
 */
static void entertainment_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Entertainment";
	mnu->messages[0] = "Internet Radio";
	mnu->messages[1] = "SD music";
	mnu->no_messages = 2 ;
	mnu->parent_ctor = main_mnu_build;
        std_mnu_buttons(mnu);
        mnu->btn_right = entertainment_btn_right;
}

/**
 * \fn entertainment_btn_right
 * \brief Move to the a child menu that plays music.
 */
static void entertainment_btn_right(struct menu* this)
{
	switch(mnu->message_id)
	{
	case 0:
		stream_menu_init();
                mnu->parent_ctor = entertainment_menu_init;
                mnu->std_child_ctor = play_menu_init;
		break;
	case 1:
                sd_menu_init();
                mnu->parent_ctor = entertainment_menu_init;
                mnu->std_child_ctor = play_menu_init;
                break;
	default:
		break;
	}
        msg_updated = TRUE;
}

/**
 * \fn stream_menu_init
 * \brief The internet stream selection menu.
 */
static void stream_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Internet Radio";
	mnu->messages[0] = "Stream";
        mnu->no_messages = 1;
	mnu->parent_ctor = entertainment_menu_init;
        std_mnu_buttons(mnu);
        mnu->btn_right = nav_to_preset_child;

}

/**
 * \fn sd_menu_init
 * \brief The SD file selection menu.
 */
static void sd_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "SD music";
	mnu->messages[0] = "bestand kiezen";
        mnu->no_messages = 1;
	mnu->parent_ctor = entertainment_menu_init;
        std_mnu_buttons(mnu);
        mnu->btn_right = nav_to_preset_child;
}

/**
 * \fn nav_to_preset_child
 * \brief Explicit set of the child to navigate to.
 */
static void nav_to_preset_child(struct menu* this)
{
        if (this->std_child_ctor != NULL)
                this->std_child_ctor();
        msg_updated = TRUE;
}

/**
 * \fn play_menu_init
 * \brief The play menu used by both stream and sd music.
 */
static void play_menu_init()
{
        if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Music is playing";
	mnu->messages[0] = "Artist - Title";
        mnu->no_messages = 1;
	mnu->parent_ctor = entertainment_menu_init;
        std_mnu_buttons(mnu);
}

/**
 * \fn alarm_menu_init
 * \brief The main alarm clock menu.
 */
static void alarm_menu_init()
{
	if(mnu == NULL)
		return;
	memset(mnu, 0, sizeof(struct menu));
	mnu->top_line = "Alarm";
	mnu->messages[0] = "Set alarm";
	mnu->messages[1] = "Set ON/OFF";
	mnu->no_messages = 2 ;
	mnu->parent_ctor = main_mnu_build;
        std_mnu_buttons(mnu);
        mnu->btn_right = main_alarm_right;
}

/**
 * \fn main_alarm_right
 * \brief Change all the alarm settings from here.
 */
static void main_alarm_right(struct menu* this)
{
        switch(mnu->message_id)
        {
        case 0:
                set_alarm_time_menu();
                break;
        case 1:
                alarm_on_off_menu();
                break;
        default:
                break;
        }
        msg_updated = TRUE;
}

/**
 * \fn set_alarm_time_menu
 * \brief The alarm time selection menu (open for changes).
 */
static void set_alarm_time_menu()
{
        if(mnu == NULL)
                return;
        memset(mnu, 0, sizeof(struct menu));
        mnu->top_line = "Set Alarm";
        mnu->messages[0] = "HH:MM";
        mnu->no_messages = 1;
        mnu->parent_ctor = alarm_menu_init;
        std_mnu_buttons(mnu);
        mnu->btn_right = select_alarm_type;
}

/**
 * \fn select_alarm_type
 * \brief Standard move to alarm type selection menu.
 */
static void select_alarm_type(struct menu* this)
{
        alarm_type_menu_init();
        msg_updated = TRUE;
}

/**
 * \fn alarm_on_off_menu
 * \brief The menu that turns the alarm on or off.
 */
static void alarm_on_off_menu()
{
        if(mnu == NULL)
                return;
        memset(mnu, 0, sizeof(struct menu));
        mnu->top_line = "Alarm ON/OFF";
        mnu->messages[0] = "ON";
        mnu->messages[1] = "OFF";
        mnu->no_messages = 2;
        mnu->parent_ctor = alarm_menu_init;
        std_mnu_buttons(mnu);
}

/**
 * \fn alarm_type_menu_init
 * \brief The alarm type selection menu; radio, sd or beep.
 */
static void alarm_type_menu_init()
{
        if(mnu == NULL)
                return;
        memset(mnu, 0, sizeof(struct menu));
        mnu->top_line = "Alarm Type";
        mnu->messages[0] = "Internet Radio";
        mnu->messages[1] = "SD Music";
        mnu->messages[2] = "Beep";
        mnu->no_messages = 3;
        mnu->parent_ctor = alarm_menu_init;
        std_mnu_buttons(mnu);
        mnu->btn_right = save_alarm_type;
}

/**
 * \fn save_alarm_type
 * \brief Move to a child that saves an alarm type setting.
 */
static void save_alarm_type(struct menu* this)
{
        switch(mnu->message_id)
        {
        case 0:
                stream_menu_init();
                mnu->parent_ctor = alarm_menu_init;
                mnu->std_child_ctor = save_menu_init;
                break;
        case 1:
                sd_menu_init();
                mnu->parent_ctor = alarm_menu_init;
                mnu->std_child_ctor = save_menu_init;
                break;
        case 2:
                save_menu_init();
                break;
        default:
                break;
        }
        msg_updated = TRUE;
}

/**
 * \fn save_menu_init
 * \brief This menu shows it when settings are saved.
 */
static void save_menu_init()
{
        if(mnu == NULL)
                return;
        memset(mnu, 0, sizeof(struct menu));
        mnu->top_line = "Settings Saved";
        mnu->messages[0] = "Settings are being saved...";
        mnu->no_messages = 1;
        mnu->parent_ctor = alarm_menu_init;
        std_mnu_buttons(mnu);
}

/**
 * \fn goose_menu_init
 * \brief The all important Goose menu.
 */
static void goose_menu_init()
{
        if(mnu == NULL)
                return;
        memset(mnu, 0, sizeof(struct menu));
        mnu->top_line = "Goose";
        mnu->messages[0] = "Quack! Quack!";
        mnu->no_messages = 1;
        mnu->parent_ctor = main_mnu_build;
        std_mnu_buttons(mnu);
}
