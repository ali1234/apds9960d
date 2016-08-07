/* apds9960d -- gesture and backlight brightness controller.
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


#include <iostream>
#include <fstream>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

#include "apds9960.h"
#include "backlight.h"
#include "uinput.h"

#include <linux/input.h>

using namespace std;

// Global Variables
APDS9960 *apds = new APDS9960(1);
Backlight *bl = new Backlight();
Uinput *ui = new Uinput();


void handleGesture()
{
    switch ( apds->readGesture() ) {
        case DIR_UP:
//            cout << "UP" << endl;
            ui->sendKey(KEY_UP);
            break;
        case DIR_DOWN:
//            cout << "DOWN" << endl;
            ui->sendKey(KEY_DOWN);
            break;
        case DIR_LEFT:
//            cout << "LEFT" << endl;
            ui->sendKey(KEY_LEFT);
            break;
        case DIR_RIGHT:
//            cout << "RIGHT" << endl;
            ui->sendKey(KEY_RIGHT);
            break;
        case DIR_SLOW:
//            cout << "SLOW" << endl;
            break;
        case DIR_FAR:
//            cout << "FAR" << endl;
            break;
        case DIR_NONE:
//            cout << "NONE" << endl;
            break;
        default:
            cout << "Unknown gesture" << endl;
    }
}


static void *interruptHandler (void *arg)
{
    int x;
    __u8 c=0;
    struct pollfd polls;

    polls.fd = (int)arg;
    polls.events = POLLPRI;

    for (;;) {
        x = poll(&polls, 1, -1);

        if(x > 0) {

          do {
            handleGesture();

            // clear interrupt and read value
            lseek (polls.fd, 0, SEEK_SET);
            read (polls.fd, &c, 1) ;
            lseek (polls.fd, 0, SEEK_SET);
            // if pin is still asserted do it again
          } while (c==1);
        }
    }
    return NULL ;
}


int main()
{
    int count, i;
    __u8 c;
    pthread_t threadId;

    ofstream gpio;

    gpio.open("/sys/class/gpio/export");
    gpio << "4" << endl;
    gpio.close();

    gpio.open("/sys/class/gpio/gpio4/direction");
    gpio << "in" << endl;
    gpio.close();

    gpio.open("/sys/class/gpio/gpio4/active_low");
    gpio << "1" << endl;
    gpio.close();

    gpio.open("/sys/class/gpio/gpio4/edge");
    gpio << "rising" << endl;
    gpio.close();

    int fd;

    fd = open("/sys/class/gpio/gpio4/value", O_RDWR);

    ioctl (fd, FIONREAD, &count);
    for (i = 0 ; i < count ; ++i)
        read (fd, &c, 1);

    pthread_create (&threadId, NULL, interruptHandler, (void *)fd) ;

    apds->enableGestureSensor(true);
    apds->enableLightSensor(false);

    while(1) {
        __u16 amb = 0;
        sleep(1);
        amb = apds->readAmbientLight();
        int target = max(12,min((int)amb,255));
        bl->setBrightness(target);
    }

    return 0;
}
