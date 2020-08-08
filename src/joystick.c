#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <hidapi/hidapi.h>

#include "mputil.h"
#include "kbd_win.h"
#include "config.h"
#include "joystick.h"

#define MAX_STR 0xff
#define BUF_SIZE 0x100

#define OJ_X 0x2
#define OJ_X_MUL 0x3
#define OJ_X_MAX 0X3FC

#define OJ_Y 0x4
#define OJ_Y_MUL 0x5
#define OJ_Y_MAX 0x3FC

#define OFFSET_THROTTLE_Y 0x6

#define P(PERCENT, MAXI) (PERCENT/100*MAXI)

struct JoystickMapping
{
    unsigned short joystick_x;
    unsigned short joystick_y;
    unsigned char throttle_y;
};


void parse_mapping(struct JoystickMapping *mapping, unsigned char (*raw_data)[BUF_SIZE])
{
    mapping->joystick_x = (*raw_data)[OJ_X + 1] + (0xff * (*raw_data)[OJ_X_MUL + 1]);
    mapping->joystick_y = (*raw_data)[OJ_Y + 1] + (0xff * (*raw_data)[OJ_Y_MUL + 1]);
    mapping->throttle_y = (*raw_data)[1 + OFFSET_THROTTLE_Y];
}

void print_mapping(struct JoystickMapping *mapping)
{
    printf("Joystick X: %02hx\n", mapping->joystick_x);
    printf("Joystick Y: %02hx\n", mapping->joystick_y);
    printf("Throttle Y: %02hhx\n", mapping->throttle_y);
}

void print_raw_buffer(unsigned char (*raw_data)[BUF_SIZE], int size)
{
    for (int i = 0; i < size; ++i)
        printf("%02hhx ", (*raw_data)[i]);
    printf("\n");
}

void print_hid_infos(hid_device *handle)
{
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

// Returns how many events have been run
int run_events(struct JoystickMapping *joystick, struct JoystickMapping *prev) {
    int cnt = 0;
    // Horn high
    if (prev->joystick_y < P(85.0, 0x3FC) && joystick->joystick_y >= P(85.0, 0x3FC)){
        ++cnt;
        printf("Horn high on\n");
        PRESS_KEY(HORN_HIGH_K);
    }
    else if (prev->joystick_y >= P(85.0, 0x3FC) && joystick->joystick_y < P(85.0, 0x3FC)){
        ++cnt;
        printf("Horn high off\n");
        RELEASE_KEY(HORN_HIGH_K);
    }
    // Horn low
    if (prev->joystick_y >= P(15.0, 0x3FC) && joystick->joystick_y < P(15.0, 0x3FC)){
        ++cnt;
        printf("Horn low on\n");
        PRESS_KEY(HORN_LOW_K);
    }
    else if (prev->joystick_y < P(15.0, 0x3FC) && joystick->joystick_y >= P(15.0, 0x3FC)){
        ++cnt;
        printf("Horn low off\n");
        RELEASE_KEY(HORN_LOW_K);
    }
    return cnt;
}

int start_joystick(int keepRunning)
{
    printf("Joystick HID controls\n");

    int res;
    unsigned char buf[BUF_SIZE];
    hid_device *handle;
    int i;

    // Initialize the hidapi library
    if (hid_init())
        return -1;

    // Open the device using the VID, PID, or serial num
    handle = hid_open(0x044F, 0xB108, NULL);

    hid_set_nonblocking(handle, 0);

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
    buf[1] = 0x0;
    res = hid_write(handle, buf, 16 + 1);
    if (res < 0)
        printf("Failed to write() (2)\n");

    struct JoystickMapping *joystick = &(struct JoystickMapping) {.joystick_x=0,.joystick_y=0,.throttle_y=0};
    struct JoystickMapping *prev_joystick = &(struct JoystickMapping) {.joystick_x=0,.joystick_y=0,.throttle_y=0};

    while (keepRunning)
    {
        res = 0;
        while (res == 0)
        {
            res = hid_read(handle, buf, sizeof(buf));
            // if (res == 0)
            //     printf("waiting...\n");
            if (res < 0)
            {
                printf("Unable to read()\n");
                break;
            }
            // SLEEP(1);
        }

        parse_mapping(joystick, &buf);


        // CLEAR();
        // print_mapping(joystick);
        // print_raw_buffer(&buf, res);

        // copy current state to previous state
        // printf("joystick:\t%p\nprev_j:\t%p\n\n", joystick, prev_joystick);

        run_events(joystick, prev_joystick);

        // Swap
        struct JoystickMapping *temp = prev_joystick;
        prev_joystick = joystick;
        joystick = temp;

    }

    return 0;
}