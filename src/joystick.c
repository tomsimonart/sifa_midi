#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <hidapi/hidapi.h>

#include "mputil.h"
#include "joystick.h"

#define MAX_STR 0xff

void print_hid_infos(hid_device *handle) {
    wchar_t wstr[MAX_STR];

    hid_get_manufacturer_string(handle, wstr, MAX_STR);
    wprintf(L"Manufacturer string: %ls\n", wstr);

    hid_get_product_string(handle, wstr, MAX_STR);
    wprintf(L"Product string: %ls\n", wstr);

    hid_get_serial_number_string(handle, wstr, MAX_STR);
    wprintf(L"Serial number: %ls\n", wstr);

    hid_get_indexed_string(handle, 1, wstr, MAX_STR);
    wprintf(L"Indexed string: %ls\n", wstr);
}

int start_joystick() {
    printf("Joystick HID controls\n");

    int res;
    unsigned char buf[0x100];
    hid_device *handle;
    int i;

    // Initialize the hidapi library
    if (hid_init())
        return -1;

    // Open the device using the VID, PID, or serial num
    handle = hid_open(0x044F, 0xB108, NULL);

    hid_set_nonblocking(handle, 1);

    print_hid_infos(handle);

    FREE_BUFFER(buf);

    buf[0] = 0x2;
    res = hid_get_feature_report(handle, buf, sizeof(buf));
    printf("Feature report\n");
    for (i = 0; i < res; i++)
        printf("%02hhx ", buf[i]);
    printf("\n");
    if (buf[1] == 0x21)
        printf("Flight stick detected\n");

    FREE_BUFFER(buf);
    buf[0] = 0x2;
    buf[1] = 0xCA;
    res = hid_write(handle, buf, 16 + 1);
    if (res < 0)
        printf("Failed to write() (2)\n");

    res = 0;
    while (res == 0) {
        res = hid_read(handle, buf, sizeof(buf));
        if (res == 0)
            printf("waiting...\n");
        if (res < 0)
            printf("Unable to read()\n");
        SLEEP(500);
    }

    for (i = 0; i < res; ++i)
        printf("%02hhx ", buf[i]);
    printf("\n");

    return 0;
}