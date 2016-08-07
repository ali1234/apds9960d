/* apds9960.cpp -- Avago APDS9960 RGB & proximity sensor driver.
 *
 * Copyright (C) 2016 Alistair Buxton <a.j.buxton@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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


#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "apds9960.h"

// This vaguely follows the API of the Sparkfun driver, but has been
// reimplemented using Linux i2c-dev and C++ features to make it
// significantly simpler.

// - Inheritance is used to provide base I2C implementation.
// - Exceptions are used instead of error code returns.
// - Macros are used to auto-generate most get/set functions.
// - Gesture processing has been rewritten from scratch.

APDS9960::APDS9960(int adapter) : I2CDevice(adapter, 0x39)
{
    // turn the chip completely off.
    write8(APDS9960_ENABLE, 0);

    // set defaults for ambient light and proximity sensors.
    write8(APDS9960_ATIME, DEFAULT_ATIME);
    write8(APDS9960_WTIME, DEFAULT_WTIME);
    write8(APDS9960_PPULSE, DEFAULT_PROX_PPULSE);
    write8(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR);
    write8(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL);
    setWaitLong(DEFAULT_WLONG);
    setLEDDrive(DEFAULT_LDRIVE);
    setProximityGain(DEFAULT_PGAIN);
    setAmbientLightGain(DEFAULT_AGAIN);
    setProximityIntLowThreshold(DEFAULT_PILT);
    setProximityIntHighThreshold(DEFAULT_PIHT);
    setLightIntLowThreshold(DEFAULT_AILT);
    setLightIntHighThreshold(DEFAULT_AIHT);
    write8(APDS9960_PERS, DEFAULT_PERS);
    write8(APDS9960_CONFIG2, DEFAULT_CONFIG2);
    write8(APDS9960_CONFIG3, DEFAULT_CONFIG3);

    // set defaults for gesture sensors.
    setGestureEnterThresh(DEFAULT_GPENTH);
    setGestureExitThresh(DEFAULT_GEXTH);
    write8(APDS9960_GCONF1, DEFAULT_GCONF1);
    setGestureGain(DEFAULT_GGAIN);
    setGestureLEDDrive(DEFAULT_GLDRIVE);
    setGestureWaitTime(DEFAULT_GWTIME);
    write8(APDS9960_GOFFSET_U, DEFAULT_GOFFSET);
    write8(APDS9960_GOFFSET_D, DEFAULT_GOFFSET);
    write8(APDS9960_GOFFSET_L, DEFAULT_GOFFSET);
    write8(APDS9960_GOFFSET_R, DEFAULT_GOFFSET);
    write8(APDS9960_GPULSE, DEFAULT_GPULSE);
    write8(APDS9960_GCONF3, DEFAULT_GCONF3);
    setGestureIntEnable(DEFAULT_GIEN);
}


APDS9960::~APDS9960()
{
    write8(APDS9960_ENABLE, 0);
}


void APDS9960::enableLightSensor(bool interrupts)
{
    setAmbientLightGain(DEFAULT_AGAIN);
    setAmbientLightIntEnable(interrupts?1:0);
    setPowerEnable(1);
    setAmbientLightEnable(1);
}


void APDS9960::disableLightSensor()
{
    setAmbientLightIntEnable(0);
    setAmbientLightEnable(0);
}


void APDS9960::enableProximitySensor(bool interrupts)
{
    setProximityGain(DEFAULT_PGAIN);
    setLEDDrive(DEFAULT_LDRIVE);
    setProximityIntEnable(interrupts?1:0);
    setPowerEnable(1);
    setProximityEnable(1);
}


void APDS9960::disableProximitySensor()
{
    setProximityIntEnable(0);
    setProximityEnable(0);
}


void APDS9960::enableGestureSensor(bool interrupts)
{
    write8(APDS9960_WTIME, 0xFF);
    write8(APDS9960_PPULSE, DEFAULT_GESTURE_PPULSE);
    setLEDBoost(LED_BOOST_300);
    setGestureIntEnable(interrupts?1:0);
    setGestureMode(1);
    setPowerEnable(1);
    setWaitEnable(1);
    setProximityEnable(1);
    setGestureEnable(1);
}


void APDS9960::disableGestureSensor()
{
    setGestureIntEnable(0);
    setGestureMode(0);
    setGestureEnable(0);
}


int APDS9960::readGesture()
{
    __u8 fifo_level = 0;
    __u8 bytes_read = 0;
    __u8 fifo_data[128];

    if( !isGestureAvailable() || !getPowerEnable() || !getGestureEnable() ) return DIR_NONE;

    gesture.clear();

    while(1) {
        if(isGestureAvailable()) {
            fifo_level = read8(APDS9960_GFLVL);
            if( fifo_level > 0) {
                bytes_read = readBlock(APDS9960_GFIFO_U, (fifo_level * 4), (__u8*)fifo_data);
                if( bytes_read == -1 ) throw bytes_read;
                for(int i = 0; i < bytes_read; i+=4) {
                    gesture.append(fifo_data+i);
                }
            }
        } else {
            return gesture.direction();
        }
        // sleep to allow fifo to refill
        std::this_thread::sleep_for(std::chrono::milliseconds(FIFO_PAUSE_TIME));
    }
}


void GestureData::clear(void)
{
    // clear buffer variables
    count = 0; overflow = false;

    for(int d=0; d<4; d++) {
        max[d] = 0; min[d] = 255;
    }
}


void GestureData::calc_delta(__u8 *data)
{
    int delta[4];

    for(int d=0; d<4; d++) {
        delta[d] = data[d] - prev[d];
        prev[d] = data[d];
    }

//    std::cout << "DATA " << int(data[0]) << ' ' << int(data[1]) << ' ' << int(data[2]) << ' ' << int(data[3]) << std::endl;

    if(delta[0] < -300) return; // this must be the first set of data

    if(delta[0] > GESTURE_DELTA && delta[1] > GESTURE_DELTA) {
        ud += data[1] - data[0];
    } else if(delta[0] < -GESTURE_DELTA && delta[1] < -GESTURE_DELTA) {
        ud += data[0] - data[1];
    }

    if(delta[2] > GESTURE_DELTA && delta[3] > GESTURE_DELTA) {
        lr += data[3] - data[2];
    } else if(delta[2] < -GESTURE_DELTA && delta[3] < -GESTURE_DELTA) {
        lr += data[2] - data[3];
    }
}


void GestureData::append(__u8 *data)
{
    if (count >= GESTURE_BUFFER_LENGTH) {
        overflow = true;
    }

    for(int d=0; d<4; d++) {
        buffer[count][d] = data[d];
        if (data[d] > max[d]) max[d] = data[d];
        if (data[d] < min[d]) min[d] = data[d];
    }
    count++;
}


int GestureData::direction(void)
{
    if(std::min(std::min(max[0], max[1]), std::min(max[2], max[3])) < 50) return DIR_FAR;

    if(overflow) return DIR_SLOW;

    // clear delta variables
    for(int d=0; d<4; d++)
        prev[d] = 1000;
    lr = 0; ud = 0;

    // process buffer
    for(int i=0; i<count; i++) {
        for(int d=0; d<4; d++) { // normalise the data points
            buffer[i][d] = (buffer[i][d] - min[d]) * 255.0 / (max[d] - min[d]);
        }
        calc_delta(buffer[i]);
    }

//    std::cout << "PLOT" << std::endl;
//    std::cout.flush();

    int xydelta = std::abs(ud) - std::abs(lr);

    if(xydelta > GESTURE_THRESH) {
        if (ud > 0)
            return DIR_DOWN;
        else
            return DIR_UP;
    } else if(xydelta < -GESTURE_THRESH){
        if (lr > 0)
            return DIR_RIGHT;
        else
            return DIR_LEFT;
    } else {
        return DIR_NONE;
    }
}
