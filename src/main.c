#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "portmidi.h"
#include "porttime.h"
#include "config.h"
#include "kbd_win.h"

// Portmidi config
#define INPUT_BUFFER_SIZE 100
#define OUTPUT_BUFFER_SIZE 0
#define DRIVER_INFO NULL
#define TIME_PROC ((int32_t (*)(void *)) Pt_Time)
#define TIME_INFO NULL
#define TIME_START Pt_Start(1, 0, 0)

// MIDI <-> SIFA config
#define PEDAL_NOTHING 0
#define PEDAL_PRESS 1
#define PEDAL_RELEASE 2

int select_device()
{
    int allowed_index = 0;
    int allowed_devices[0xff];

    // List device info for all MIDI inputs
    int default_input_device = Pm_GetDefaultInputDeviceID();
    for (int i = 0; i < Pm_CountDevices(); ++i)
    {
        const PmDeviceInfo *device_info = Pm_GetDeviceInfo(i);
        if (device_info->input)
        {
            allowed_devices[allowed_index] = i;
            allowed_index++;

            printf("%d: %s, %s", i, device_info->interf, device_info->name);
            if (i == default_input_device)
            {
                printf(" default\n");
            }
            else
            {
                printf("\n");
            }
        }
    }

    int choice;
    while (1)
    {
        printf("Enter the device ID: ");
        scanf("%d", &choice);
        for (int i = 0; i < allowed_index; ++i)
        {
            if (choice == allowed_devices[i])
            {
                return choice;
            }
        }
        printf("Wrong choice, please re-try.\n");
    }

    return -1;
}

int is_pedal_action(long status, long data_1, long data_2) {
    if (status == MIDI_KEY_STATUS && data_1 == MIDI_KEY_DATA_1) {
        if (data_2 == MIDI_KEY_DATA_2_PRESSED) {
            return PEDAL_PRESS;
        }
        else if (data_2 == MIDI_KEY_DATA_2_RELEASED) {
            return PEDAL_RELEASE;
        }
    }
    return PEDAL_NOTHING;
}


int main()
{
    int midi_device = select_device();
    const PmDeviceInfo *info = Pm_GetDeviceInfo(midi_device);
    printf("%s\n", info->name);

    PmStream *midi_stream;
    PmError length;
    PmEvent buffer[1];

    // Start a midi timer with a millisecond accuracy
    TIME_START;

    // Open the input device
    printf("Opening MIDI stream with device...\n");
    Pm_OpenInput(
        &midi_stream,
        midi_device,
        DRIVER_INFO,
        INPUT_BUFFER_SIZE,
        TIME_PROC,
        TIME_INFO);
    printf("Done.\n");

    // Set a filter for the received data
    Pm_SetFilter(midi_stream, PM_FILT_ACTIVE || PM_FILT_CLOCK || PM_FILT_SYSEX);

    // Empty the buffer after setting the filter
    while (Pm_Poll(midi_stream)) {
        Pm_Read(midi_stream, buffer, 1);
    }


    // Receive messages
    while (1) {
        Sleep(SLEEP);
        if (Pm_Poll(midi_stream) == TRUE) {
            length = Pm_Read(midi_stream, buffer, 1);
            if (length > 0) {
                int pedal_action = is_pedal_action(
                    (long) Pm_MessageStatus(buffer[0].message),
                    (long) Pm_MessageData1(buffer[0].message),
                    (long) Pm_MessageData2(buffer[0].message));
                
                if (pedal_action == PEDAL_PRESS) {
                    printf("Pedal pressed.\n");
                    send_key_release();
                }
                else if (pedal_action == PEDAL_RELEASE) {
                    printf("Pedal released.\n");
                    send_key_press();
                }
            }
            else
            {
                assert(0);
            }
        }
    }

    // Close the device
    printf("Closing MIDI stream with device...\n");
    Pm_Close(midi_stream);
    printf("Done.\n");

    return 0;
}