#ifndef SIFA_MIDI_CONFIG_H
#define SIFA_MIDI_CONFIG_H

// Key to virtually press
#define SIFA_K                      0x51
#define HORN_HIGH_K                 0x4E
#define HORN_LOW_K                  0x20

// Midi input to get
#define MIDI_KEY_STATUS             0xb0
#define MIDI_KEY_DATA_1             0x40
#define MIDI_KEY_DATA_2_PRESSED     0x7f
#define MIDI_KEY_DATA_2_RELEASED    0x00

// Midi input loop config
#define MIDI_LOOP_DELAY_MS          50

#endif