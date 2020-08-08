#ifndef KBD_WIN
#define KBD_WIN

#include <windows.h>

#include "config.h"

#ifdef WIN32
    #define PRESS_KEY(KEYCODE) win_send_key_press(KEYCODE)
    #define RELEASE_KEY(KEYCODE) win_send_key_release(KEYCODE)

    void win_send_key_press(WORD keycode);
    void win_send_key_release(WORD keycode);
#else
#endif


#endif