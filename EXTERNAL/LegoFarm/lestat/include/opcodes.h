/**
 *
 * Copyright 2008 Nicolas A. Barriga <nbarriga@inf.utfsm.cl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef __OPCODES_H__
#define __OPCODES_H__

#include "comms.h"
#include "exceptions.h"
#include "message.h"

/** 
 * Structure returned by Opcodes::getOutputState
 */
struct MotorState{
    unsigned char port;
    char power;
    unsigned char mode;
    unsigned char regMode;
    char turnRatio;
    unsigned char runState;
    unsigned int tachoLimit;
    int tachoCount;
    int blockTachoCount;
    int rotationCount;
};
enum SensorType {NO_SENSOR=0x00, SWITCH=0x01, TEMPERATURE=0x02, REFLECTION=0x03, ANGLE=0x04, LIGHT_ACTIVE=0x05, LIGHT_INACTIVE=0x06, SOUND_DB=0x07, SOUND_DBA=0x08, CUSTOM=0x09, LOWSPEED=0x0a, LOWSPEED_9V=0x0b, NO_OF_SENSOR_TYPES=0x0c};

enum SensorMode{RAWMODE=0x00, BOOLEANMODE=0x20, TRANSITIONCNTMODE=0x40, PERIODCOUNTERMODE=0x60, PCTFULLSCALEMODE=0x80, CELSIUSMODE=0xa0, FAHRENHEITMODE=0xc0, ANGLESTEPMODE=0xe0, SLOPEMASK=0x1f, MODEMASK=0xe0};//TODO: ANGLESTEPMODE and MODEMASK have the same value. Document is wrong


/**
 *  * Structure returned by Opcodes::getInputState
 *   */
struct SensorState{
    unsigned char port;
    bool valid;
    bool calibrated;
    SensorType type;
    SensorMode mode;
    unsigned short int rawValue;
    unsigned short int normalizedValue;
    short int scaledValue;
    short int calibratedValue;
};

class ErrorCodes{
    public:
        static string getErrorMessage(char code);
};
class Opcodes{
    private:
        Comms *nxt;
    public:
        Opcodes(Comms *nxt);
        void playTone(unsigned short int freq, unsigned short int time, bool answer=false, unsigned char *status=NULL);
        unsigned short int getBatteryLevel();
        /**
         * Set output state of the motors
         *  @param port Output port: 0 -> A, 1 -> B, 2 -> C, 0xFF -> ALL
         *  @param power Power set point(Range: -100 - 100)
         *  @param mode Mode byte(0x01: turn on the specified motor, 0x02: Use run/brake instead of run/float in PWM, 0x04: Turns on the regulation)
         *  @param regMode Regulation mode(0x00: disabled, 0x01: Power control enabled, 0x02: Synchronization enabled(Needs enabled on two output))
         *  @param turnRatio Turn Ratio(-100 - 100)
         *  @param runState (0x00: idle, 0x10: ramp-up, 0x20: running, 0x40:ramp-down)
         *  @param tachoLimit (0, run forever)
         *  @param answer if true, request answer from brick
         *  @param status must be not null and reserved if answer==true
         *
         *
         */
        void setOutputState(unsigned char port, char power, unsigned char mode, unsigned char regMode, char turnRatio, unsigned char runState, unsigned int tachoLimit, bool answer=false, unsigned char *status=NULL);
        /**
         * Get motor states
         *
         * @param port Output port: 0 -> A, 1 -> B, 2 -> C
         */
        MotorState getOutputState(unsigned char port);

        /**
         * Set the type and mode of the sensor plugged to a given port
         *
         * @param port Input port(Range: 0 - 3)
         * @param type Sensor type (enumerated)
         * @param mode Sensor mode (enumerated)
         */
        void setInputMode(unsigned char port, SensorType type, SensorMode mode, bool answer=false, unsigned char *status=NULL);


        SensorState getInputValues(unsigned char port);

        /**
         * Resets the motor counter
         *
         * @param port Input port(Range: 0 - 2)
         * @param relative True: reset position relative to last movement, False: reset absolute position
         */
        void resetMotorPosition(unsigned char port, bool relative, bool answer=false, unsigned char *status=NULL);
/*
0x00: (start_program, _parse_simple),
0x01: (stop_program, _parse_simple),
0x02: (play_sound_file, _parse_simple),
0x03: (play_tone, _parse_simple),
0x04: (set_output_state, _parse_simple),
0x05: (set_input_mode, _parse_simple),
0x06: (get_output_state, _parse_get_output_state),
0x07: (get_input_values, _parse_get_input_values),
0x08: (reset_input_scaled_value, _parse_simple),
0x09: (message_write, _parse_simple),
0x0A: (reset_motor_position, _parse_simple),
0x0B: (get_battery_level, _parse_get_battery_level),
0x0C: (stop_sound_playback, _parse_simple),
0x0D: (keep_alive, _parse_keep_alive),
0x0E: (ls_get_status, _parse_ls_get_status),
0x0F: (ls_write, _parse_simple),
0x10: (ls_read, _parse_ls_read),
0x11: (get_current_program_name, _parse_get_current_program_name),
0x13: (message_read, _parse_message_read),
*/

};

#endif /* __OPCODES_H__ */
