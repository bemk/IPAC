#ifndef _MENU_H
#define _MENU_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct menu
 * \brief The descriptor for the menu items
 */

struct menu {
        /**
         * \fn btn1
         * \fn btn2
         * \fn btn3
         * \fn btn4
         * \fn btn5
         * \fn btn_ok
         * \fn btn_esc
         * \fn btn_up
         * \fn btn_down
         * \fn btn_left
         * \fn btn_right
         * \fn btn_alt
         */
        void (*btn1)(struct menu*);
        void (*btn2)(struct menu*);
        void (*btn3)(struct menu*);
        void (*btn4)(struct menu*);
        void (*btn5)(struct menu*);
        void (*btn_ok)(struct menu*);
        void (*btn_esc)(struct menu*);
        void (*btn_up)(struct menu*);
        void (*btn_down)(struct menu*);
        void (*btn_left)(struct menu*);
        void (*btn_right)(struct menu*);
        void (*btn_alt)(struct menu*);
        /**
         * \fn parent_ctor
         * \brief When we need to go one menu up, we call this function
         */
        void (*parent_ctor)(struct menu*);

        /**
         * \var top_line
         * \brief The stationary line at the top of the screen
         * \var messages
         * \brief The itterating (scrolling) messages
         * \var message_id
         * \brief Which itterating message is currently visible
         * \var no_messages
         * \brief How many messages do we have
         */

        char* top_line;
        char** messages;
        int message_id;
        int no_messages;
};

#ifdef __cplusplus
}
#endif

#endif
