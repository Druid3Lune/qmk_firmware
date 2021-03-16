/* Copyright 2019-2020 DMQ Design
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "midi.h"
#include "qmk_midi.h"

uint8_t currentLayer;
uint8_t midi1 = 64;
uint8_t midi2 = 64;
uint8_t midi3 = 64;
uint8_t midi_channel = 0;
char midivalstr[7] = "   ";

enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
};

static oled_rotation_t rotation = OLED_ROTATION_0;

static void dance_oled_finished(void) {
    switch (rotation) {
        case OLED_ROTATION_0:
            rotation = OLED_ROTATION_90;
            break;
        case OLED_ROTATION_90:
            rotation = OLED_ROTATION_180;
            break;
        case OLED_ROTATION_180:
            rotation = OLED_ROTATION_270;
            break;
        default:
            rotation = OLED_ROTATION_0;
            break;
    }
    oled_init(rotation);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case QMKBEST:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            dance_oled_finished();
            SEND_STRING("");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(/* Base */
                KC_KP_7, KC_KP_8, KC_KP_9, TO(0),
                KC_KP_4, KC_KP_5, KC_KP_6, TO(1),
                KC_KP_1, KC_KP_2, KC_KP_3, TO(2),
                KC_KP_0, RGB_TOG, RGB_MOD
                ),

    [1] = LAYOUT(/* Base */
                KC_NO, KC_NO, KC_NO, KC_TRNS,
                KC_NO, KC_NO, KC_NO, KC_TRNS,
                KC_NO, KC_NO, KC_NO, KC_TRNS,
                MI_OCTD, QMKBEST, MI_OCTU
                ),g
    [2] = LAYOUT(/* Base */
                MI_C_1,  MI_Cs_1, MI_D_1, KC_TRNS,
                MI_Gs, MI_A,  MI_As, KC_TRNS,
                MI_E,  MI_F,  MI_Fs, KC_TRNS,
                MI_C,  MI_Cs, MI_D
                )
};

// #ifdef OLED_DRIVER_ENABLE

// static void render_logo(void) {
//     static const char PROGMEM qmk_logo[] = {
//         0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
//         0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
//         0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
//     };

//     oled_write_P(qmk_logo, false);
// }

// static void tes_oled(void) {
//     uint8_t x = 32;
//     uint8_t y = 128;

// }

extern OLED_BLOCK_TYPE oled_dirty;

static inline uint8_t pixel_width(void) {
    if (!(rotation & OLED_ROTATION_90)) {
        return OLED_DISPLAY_WIDTH;
    }
    return OLED_DISPLAY_HEIGHT;
}

static inline uint8_t pixel_height(void) {
    if (!(rotation & OLED_ROTATION_90)) {
        return OLED_DISPLAY_HEIGHT;
    }
    return OLED_DISPLAY_WIDTH;
}

// Draw the QMK logo at the top left corner, clipping if it does not fit.
static void test_logo(void) {
    uint8_t lines = oled_max_lines();
    if (lines > 3) {
        lines = 3;
    }
    uint8_t chars = oled_max_chars();
    if (chars > 21) {
        chars = 21;
    }
    for (uint8_t row = 0; row < lines; ++row) {
        oled_set_cursor(0, row);
        for (uint8_t col = 0; col < chars; ++col) {
            oled_write_char(0x80 + 0x20 * row + col, false);
        }
    }
}

// static const PROGMEM char fill_ff[OLED_MATRIX_SIZE] = {[0 ... OLED_MATRIX_SIZE - 1] = 0xff};

// Fill the whole screen with a pattern made from two bytes alternating after the specified number of repeats.
// static void test_fill(uint8_t byte0, uint8_t byte1, uint8_t repeats) {
//     uint8_t  width = pixel_width();
//     uint8_t  lines = oled_max_lines();
//     uint16_t index = 0;
//     for (uint8_t row = 0; row < lines; ++row) {
//         for (uint8_t col = 0; col < width; ++col) {
//             uint8_t byte = ((col / repeats) % 2) ? byte1 : byte0;
//             oled_write_raw_byte(byte, index++);
//         }
//     }
// }
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return rotation;
}
void oled_task_user(void) {
    // Host Keyboard Layer Status
    test_logo();

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("midi chan\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("midi enc.\n"), false);
            break;
     }

    // // // // Host Keyboard LED Status
    // led_t led_state = host_keyboard_led_state();
    // oled_write_P(led_state.num_lock ? PSTR("NUM \n") : PSTR("    \n"), false);
    // oled_write_P(led_state.caps_lock ? PSTR("CAP \n") : PSTR("    \n"), false);
    // oled_write_P(led_state.scroll_lock ? PSTR("SCR \n") : PSTR("    \n"), false);
}

void encoder_update_user(uint8_t index, bool clockwise) {
    switch (currentLayer){
    case 0: /*first layer*/
        switch (index){
        case 0: /*1st encoder*/
            if (clockwise) {
                rgblight_increase_hue(); //Cycle through the RGB hue
            } else {
                rgblight_decrease_hue();
            }
            break;
        case 1:  /*2nd encoder*/
            if (clockwise) {
                tap_code(KC_VOLU); //Example of using tap_code which lets you use keycodes outside of the keymap
            } else {
                tap_code(KC_VOLD);
            }
            break;
        case 2: /*3rd encoder*/
            if (clockwise) {
                rgblight_increase_val(); //Change brightness on the RGB LEDs
            } else {
                rgblight_decrease_val();
            }
            break;
        }
        break;
    case 1: /*second layer*/
        switch (index){
        case 0: /*1st encoder*/
            if (clockwise) {
                midi_channel = midi_channel < 15 ? midi1 +1 : 15;
            } else {
                midi_channel = midi_channel > 0 ? midi_channel -1 : 0;
            }
            // itoa(midi_channel,midivalstr, 10);
            // oled_write_P(midivalstr, false);
            // oled_write_P(PSTR(midivalstr), false);
            break;
        case 1:  /*2nd encoder*/
            if (clockwise) {
                tap_code(KC_VOLU); //Example of using tap_code which lets you use keycodes outside of the keymap
            } else {
                tap_code(KC_VOLD);
            }
            break;
        case 2: /*3rd encoder*/
            if (clockwise) {
                rgblight_increase_val(); //Change brightness on the RGB LEDs
            } else {
                rgblight_decrease_val();
            }
            break;
        }
        break;
    case 2:
            switch (index){
        case 0: /*1st encoder*/
            if (clockwise) {
                midi1 = midi1 < 127 ? midi1 +1 : 127;
            } else {
                midi1 = midi1 > 0 ? midi1 -1 : 0;
            }
            midi_send_cc(&midi_device, midi_channel, 23,midi1);
            // itoa(midi1,midivalstr, 10);
            // oled_write_P(midivalstr, false);
            // oled_write_P(PSTR(midivalstr), false);
            break;
        case 1:  /*2nd encoder*/
            if (clockwise) {
                midi2 = midi2 < 127 ? midi2 + 1 : 127;
            } else {
                midi2 = midi2 > 0 ? midi2 -1 : 0;
            }
            midi_send_cc(&midi_device, midi_channel, 24,midi2);
            // itoa(midi2,midivalstr, 10);
            // oled_write_P(midivalstr, false);
            // oled_write_P(PSTR(midivalstr), false);
            break;
        case 2: /*3rd encoder*/
            if (clockwise) {
                midi3 = midi3 < 127 ? midi3 +1 : 127;
            } else {
                midi3 = midi3 > 0 ? midi3 -1 : 0;
            }
            midi_send_cc(&midi_device, midi_channel, 25,midi3);
            // itoa(midi3,midivalstr, 10);
            // oled_write_P(midivalstr, false);
            // oled_write_P(PSTR(midivalstr), false);
            break;
        }
        break;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) { //This will run every time the layer is updated
    currentLayer = get_highest_layer(state);
    switch (currentLayer) {
        case 0:
            setrgb(RGB_WHITE, &led[0]); //Set the top LED to white for the bottom layer
            setrgb(0, 0, 0, &led[1]);
            setrgb(0, 0, 0, &led[2]);
            break;
        case 1:
            setrgb(0, 0, 0, &led[0]); //Set the middle LED to white for the middle layer
            setrgb(RGB_WHITE, &led[1]);
            setrgb(0, 0, 0, &led[2]);
            break;
        case 2:
            setrgb(0, 0, 0, &led[0]);
            setrgb(0, 0, 0, &led[1]);
            setrgb(RGB_WHITE, &led[2]); //Set the bottom LED to white for the top layer
            break;
    }
    rgblight_set();
    return state;
}
