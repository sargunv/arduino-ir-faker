#pragma once

#include "secrets.h"

/*
 * Usually with a TRS IR Ext port, tip is +, sleeve is -, and ring 1 is the
 * non-modulated IR signal. If modding in your own port, you can optionally
 * put something else on ring 2 of a TRRS port.
 *
 * Here we'll say RING_1_PIN is the IR signal, and RING_2_PIN is the
 * optional extra input.
 *
 * Before assembly, ensure the signal voltage of your Arduino is the same
 * as that of the device you're connecting it to. If it's not, you may want
 * to use an optocoupler instead for communication.
 *
 * I use RING_2_PIN to get the status of the power LED on an HDMI switch.
 *
 * The sleeve will be connected to ground on our Arduino, and the tip
 * will be left floating as our Arduino is externally powered over USB.
 */

#define RING_1_PIN 2
#define RING_2_PIN 3

/*
 * Arduino-IRremote options
 * https://github.com/Arduino-IRremote/Arduino-IRremote#compile-options--macros-for-this-library
 */
#define DEBUG true
#define USE_NO_SEND_PWM
#define IR_SEND_PIN RING_1_PIN
