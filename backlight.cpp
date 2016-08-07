/* backlight.cpp -- backlight brightness controller.
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
#include <thread>

#include "backlight.h"

Backlight::Backlight()
{
    blb.open("/sys/class/backlight/rpi_backlight/brightness");
    blb >> current_brightness;
}


Backlight::~Backlight()
{
    blb.close();
}


void Backlight::setBrightness(int target)
{
    target = std::min(255, std::max(0, target));
    while(current_brightness != target)
    {
        if (current_brightness < target) current_brightness++;
        else if (current_brightness > target) current_brightness--;

        blb << current_brightness << std::endl;

        int delay = 255 - current_brightness;
        std::this_thread::sleep_for(std::chrono::microseconds(delay*delay/2));
    }
}
