#ifndef I2CDEVICE_H
#define I2CDEVICE_H

#include <linux/i2c-dev.h>

#define BYTESWAP(x) ( ((x&0xff)<<8) | ((x&0xff00)>>8) )

class I2CDevice
{
    public:
        I2CDevice(int adapter, int address);
        ~I2CDevice();

    protected:
        ssize_t readRaw(__u8 length, __u8* values);
        ssize_t writeRaw(__u8 length, const __u8* values);

        __u8 read8(__u8 reg);
        void write8(__u8 reg, __u8 val);

        __u16 read16(__u8 reg);
        void write16(__u8 reg, __u16 val);

        __s32 readBlock(__u8 reg, __u8 length, __u8* values);

        __u8 readBit(__u8 reg, __u8 shift);
        void writeBit(__u8 reg, __u8 value, __u8 shift);

        __u8 readBits(__u8 reg, __u8 shift, __u8 bits);
        void writeBits(__u8 reg, __u8 value, __u8 shift, __u8 bits);

    private:
        int file_des;
};

#define I2CDEVICE_REG_8(name, reg) \
    __u8 get##name(void) { read8(reg); } \
    void set##name(__u8 val) { write8(reg, val); }

#define I2CDEVICE_REG_16(name, reg) \
    __u16 get##name(void) { read16(reg); } \
    void set##name(__u16 val) { write16(reg, val); }

#define I2CDEVICE_REG_1(name, reg, shift) \
    __u8 get##name(void) { readBit(reg, shift); } \
    void set##name(__u8 val) { writeBit(reg, val, shift); }

#define I2CDEVICE_REG_N(name, reg, shift, bits) \
    __u8 get##name(void) { readBits(reg, shift, bits); } \
    void set##name(__u8 val) { writeBits(reg, val, shift, bits); }

#endif // I2CDEVICE_H
