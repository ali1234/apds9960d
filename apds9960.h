#ifndef APDS9960_H
#define APDS9960_H

#include "i2cdevice.h"

// definitions borrowed from SparkFun Electronics APDS9960 Arduino library.

/* APDS-9960 I2C address */
#define APDS9960_I2C_ADDR       0x39

/* Gesture parameters */
#define GESTURE_THRESHOLD_OUT   10
#define GESTURE_SENSITIVITY_1   50
#define GESTURE_SENSITIVITY_2   20

/* Acceptable device IDs */
#define APDS9960_ID_1           0xAB
#define APDS9960_ID_2           0x9C

/* Misc parameters */
#define FIFO_PAUSE_TIME         10      // Wait period (ms) between FIFO reads

/* APDS-9960 register addresses */
#define APDS9960_ENABLE         0x80
#define APDS9960_ATIME          0x81
#define APDS9960_WTIME          0x83
#define APDS9960_AILTL          0x84
#define APDS9960_AILTH          0x85
#define APDS9960_AIHTL          0x86
#define APDS9960_AIHTH          0x87
#define APDS9960_PILT           0x89
#define APDS9960_PIHT           0x8B
#define APDS9960_PERS           0x8C
#define APDS9960_CONFIG1        0x8D
#define APDS9960_PPULSE         0x8E
#define APDS9960_CONTROL        0x8F
#define APDS9960_CONFIG2        0x90
#define APDS9960_ID             0x92
#define APDS9960_STATUS         0x93
#define APDS9960_CDATAL         0x94
#define APDS9960_CDATAH         0x95
#define APDS9960_RDATAL         0x96
#define APDS9960_RDATAH         0x97
#define APDS9960_GDATAL         0x98
#define APDS9960_GDATAH         0x99
#define APDS9960_BDATAL         0x9A
#define APDS9960_BDATAH         0x9B
#define APDS9960_PDATA          0x9C
#define APDS9960_POFFSET_UR     0x9D
#define APDS9960_POFFSET_DL     0x9E
#define APDS9960_CONFIG3        0x9F
#define APDS9960_GPENTH         0xA0
#define APDS9960_GEXTH          0xA1
#define APDS9960_GCONF1         0xA2
#define APDS9960_GCONF2         0xA3
#define APDS9960_GOFFSET_U      0xA4
#define APDS9960_GOFFSET_D      0xA5
#define APDS9960_GOFFSET_L      0xA7
#define APDS9960_GOFFSET_R      0xA9
#define APDS9960_GPULSE         0xA6
#define APDS9960_GCONF3         0xAA
#define APDS9960_GCONF4         0xAB
#define APDS9960_GFLVL          0xAE
#define APDS9960_GSTATUS        0xAF
#define APDS9960_IFORCE         0xE4
#define APDS9960_PICLEAR        0xE5
#define APDS9960_CICLEAR        0xE6
#define APDS9960_AICLEAR        0xE7
#define APDS9960_GFIFO_U        0xFC
#define APDS9960_GFIFO_D        0xFD
#define APDS9960_GFIFO_L        0xFE
#define APDS9960_GFIFO_R        0xFF

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* Proximity Gain (PGAIN) values */
#define PGAIN_1X                0
#define PGAIN_2X                1
#define PGAIN_4X                2
#define PGAIN_8X                3

/* ALS Gain (AGAIN) values */
#define AGAIN_1X                0
#define AGAIN_4X                1
#define AGAIN_16X               2
#define AGAIN_64X               3

/* Gesture Gain (GGAIN) values */
#define GGAIN_1X                0
#define GGAIN_2X                1
#define GGAIN_4X                2
#define GGAIN_8X                3

/* LED Boost values */
#define LED_BOOST_100           0
#define LED_BOOST_150           1
#define LED_BOOST_200           2
#define LED_BOOST_300           3

/* Gesture wait time values */
#define GWTIME_0MS              0
#define GWTIME_2_8MS            1
#define GWTIME_5_6MS            2
#define GWTIME_8_4MS            3
#define GWTIME_14_0MS           4
#define GWTIME_22_4MS           5
#define GWTIME_30_8MS           6
#define GWTIME_39_2MS           7

/* Default values */
#define DEFAULT_ATIME           0xFF    // 1 cycle, 2.78ms
#define DEFAULT_WTIME           0xFF    // 1 cycle, 2.78ms
#define DEFAULT_PROX_PPULSE     0x87    // 16us, 8 pulses
#define DEFAULT_GESTURE_PPULSE  0x89    // 16us, 10 pulses
#define DEFAULT_POFFSET_UR      0       // 0 offset
#define DEFAULT_POFFSET_DL      0       // 0 offset
#define DEFAULT_WLONG           1       // No 12x wait (WTIME) factor
#define DEFAULT_LDRIVE          LED_DRIVE_100MA
#define DEFAULT_PGAIN           PGAIN_4X
#define DEFAULT_AGAIN           AGAIN_64X
#define DEFAULT_PILT            0       // Low proximity threshold
#define DEFAULT_PIHT            50      // High proximity threshold
#define DEFAULT_AILT            0xFFFF  // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x11    // 2 consecutive prox or ALS for int.
#define DEFAULT_CONFIG2         0x01    // No saturation interrupts or LED boost
#define DEFAULT_CONFIG3         0       // Enable all photodiodes, no SAI
#define DEFAULT_GPENTH          40      // Threshold for entering gesture mode
#define DEFAULT_GEXTH           30      // Threshold for exiting gesture mode
#define DEFAULT_GCONF1          0x40    // 4 gesture events for int., 1 for exit
#define DEFAULT_GGAIN           GGAIN_4X
#define DEFAULT_GLDRIVE         LED_DRIVE_100MA
#define DEFAULT_GWTIME          GWTIME_5_6MS
#define DEFAULT_GOFFSET         0       // No offset scaling for gesture mode
#define DEFAULT_GPULSE          0xC9    // 32us, 10 pulses
#define DEFAULT_GCONF3          0       // All photodiodes active during gesture
#define DEFAULT_GIEN            0       // Disable gesture interrupts


enum { DIR_NONE, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_SLOW, DIR_FAR };

#define GESTURE_DELTA (10)
#define GESTURE_THRESH (100)
#define GESTURE_BUFFER_LENGTH (256)

class GestureData {
public:
    void clear(void);
    void append(__u8 *data);
    int direction(void);
    void calc_delta(__u8 *data);

private:
    __u8 buffer[GESTURE_BUFFER_LENGTH][4];
    __u8 min[4];
    __u8 max[4];
    int count;
    bool overflow;

    int prev[4];
    int lr;
    int ud;
};


class APDS9960 : public I2CDevice {
public:
    APDS9960(int);
    ~APDS9960();

    void enableLightSensor(bool interrupts = false);
    void disableLightSensor();
    void enableProximitySensor(bool interrupts = false);
    void disableProximitySensor();
    void enableGestureSensor(bool interrupts = true);
    void disableGestureSensor();

    I2CDEVICE_REG_N(LEDDrive, APDS9960_CONTROL, 6, 2)
    I2CDEVICE_REG_N(GestureLEDDrive, APDS9960_GCONF2, 3, 2)

    I2CDEVICE_REG_N(AmbientLightGain, APDS9960_CONTROL, 0, 2)
    I2CDEVICE_REG_N(ProximityGain, APDS9960_CONTROL, 2, 2)
    I2CDEVICE_REG_N(GestureGain, APDS9960_GCONF2, 5, 2)

    I2CDEVICE_REG_16(LightIntLowThreshold, APDS9960_AILTL)
    I2CDEVICE_REG_16(LightIntHighThreshold, APDS9960_AIHTL)
    I2CDEVICE_REG_16(ProximityIntLowThreshold, APDS9960_PILT)
    I2CDEVICE_REG_16(ProximityIntHighThreshold, APDS9960_PIHT)

    I2CDEVICE_REG_1(GestureIntEnable, APDS9960_GCONF4, 1)

    void clearAmbientLightInt() { read8(APDS9960_AICLEAR); }
    void clearProximityInt() { read8(APDS9960_PICLEAR); }

    __u16 readAmbientLight(void) { return read16(APDS9960_CDATAL); }
    __u16 readRedLight(void) { return read16(APDS9960_RDATAL); }
    __u16 readGreenLight(void) { return read16(APDS9960_GDATAL); }
    __u16 readBlueLight(void) { return read16(APDS9960_BDATAL); }
    __u8 readProximity(void) { return read16(APDS9960_PDATA); }

    bool isGestureAvailable() { return readBit(APDS9960_GSTATUS, 0); }
    int readGesture();

private:
    I2CDEVICE_REG_1(PowerEnable, APDS9960_ENABLE, 0);
    I2CDEVICE_REG_1(AmbientLightEnable, APDS9960_ENABLE, 1);
    I2CDEVICE_REG_1(ProximityEnable, APDS9960_ENABLE, 2);
    I2CDEVICE_REG_1(WaitEnable, APDS9960_ENABLE, 3);
    I2CDEVICE_REG_1(WaitLong, APDS9960_CONFIG1, 1);
    I2CDEVICE_REG_1(AmbientLightIntEnable, APDS9960_ENABLE, 4);
    I2CDEVICE_REG_1(ProximityIntEnable, APDS9960_ENABLE, 5);
    I2CDEVICE_REG_1(GestureEnable, APDS9960_ENABLE, 6);

    I2CDEVICE_REG_N(LEDBoost, APDS9960_CONFIG2, 4, 2);

    I2CDEVICE_REG_1(ProxGainCompEnable, APDS9960_CONFIG3, 5);
    I2CDEVICE_REG_N(ProxPhotoMask, APDS9960_CONFIG3, 0, 4);

    I2CDEVICE_REG_8(GestureEnterThresh, APDS9960_GPENTH);
    I2CDEVICE_REG_8(GestureExitThresh, APDS9960_GEXTH);

    I2CDEVICE_REG_N(GestureWaitTime, APDS9960_GCONF2, 0, 3);

    I2CDEVICE_REG_1(GestureMode, APDS9960_GCONF4, 0);

    GestureData gesture;
};

#endif // APDS9960_H
