#include "kbd_win.h"

void send_key_press() {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = KEYCODE;
    ip.ki.wScan = 0;
    ip.ki.dwFlags = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    SendInput(1, &ip, sizeof(INPUT));
}

void send_key_release() {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = KEYCODE;
    ip.ki.wScan = 0;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    SendInput(1, &ip, sizeof(INPUT));
}