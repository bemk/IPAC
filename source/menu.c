#include "application.h"
#include "menu.h"

<<<<<<< HEAD
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
        if(this->message_id >= this->no_messages)
                this->message_id = this->no_messages;
}

/**
 * \fn std_btn_left
 * \brief Standard move to the parent menu
 */
void std_btn_left(struct menu* this)
{
        this->parent_ctor(this);
}

void std_btn_right(struct menu* this)
{
}
=======
>>>>>>> f8a07e6... mnu: Made a menu item data structure (with Remy)
