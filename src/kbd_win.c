#include "kbd_win.h"

void win_send_key_press(WORD keycode) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = keycode;
    ip.ki.wScan = 0;
    ip.ki.dwFlags = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    SendInput(1, &ip, sizeof(INPUT));
}

void win_send_key_release(WORD keycode) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = keycode;
    ip.ki.wScan = 0;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    SendInput(1, &ip, sizeof(INPUT));
}

