/* i2cdevice.cpp -- i2c device base class
 *
 * Copyright (C) 2015,2016 Alistair Buxton <a.j.buxton@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "i2cdevice.h"

I2CDevice::I2CDevice(int adapter, int address)
{
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", adapter);
    file_des = open(filename, O_RDWR|O_NONBLOCK);
    if (file_des < 0) {
        fprintf(stderr, "Error opening i2c adapter.\n");
        throw file_des;
    }
    if (ioctl(file_des, I2C_SLAVE, address) < 0) {
        fprintf(stderr, "Error setting i2c device address.\n");
        throw file_des;
    }
}

I2CDevice::~I2CDevice()
{
    close(file_des);
}



ssize_t I2CDevice::readRaw(__u8 length, __u8* values)
{
    return read(file_des, values, length);
}

ssize_t I2CDevice::writeRaw(__u8 length, const __u8* values)
{
    return write(file_des, values, length);
}

__u8 I2CDevice::read8(__u8 reg)
{
    __s32 result = i2c_smbus_read_byte_data(file_des, reg);
    if(result < 0)
        throw result;
    else
        return result;
}

void I2CDevice::write8(__u8 reg, __u8 val)
{
    __s32 result = i2c_smbus_write_byte_data(file_des, reg, val);
    if(result < 0)
        throw result;
}

__u16 I2CDevice::read16(__u8 reg)
{
    __s32 result = i2c_smbus_read_word_data(file_des, reg);
    if(result < 0)
        throw result;
    else
        return result;
}

void I2CDevice::write16(__u8 reg, __u16 val)
{
    __s32 result = i2c_smbus_write_word_data(file_des, reg, val);
    if(result < 0)
        throw result;
}

__s32 I2CDevice::readBlock(__u8 reg, __u8 length, __u8* values)
{
    return i2c_smbus_read_i2c_block_data(file_des, reg, length, values);
}

__u8 I2CDevice::readBit(__u8 reg, __u8 shift)
{
    return (read8(reg) >> shift) & 1;
}

void I2CDevice::writeBit(__u8 reg, __u8 value, __u8 shift)
{
    __u8 tmp = read8(reg);

    __u8 bits = 1 << shift;
    value = value << shift;
    tmp = (tmp & ~bits) | (value & bits);

    write8(reg, tmp);
}

__u8 I2CDevice::readBits(__u8 reg, __u8 shift, __u8 bits)
{
    bits = ((1<<bits)-1);

    return (read8(reg) >> shift) & bits;
}

void I2CDevice::writeBits(__u8 reg, __u8 value, __u8 shift, __u8 bits)
{
    __u8 tmp = read8(reg);

    bits = ((1<<bits)-1)<<shift;
    value = value << shift;
    tmp = (tmp & ~bits) | (value & bits);

    write8(reg, tmp);
}
