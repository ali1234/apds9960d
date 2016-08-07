#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <fstream>

class Backlight
{
    public:
        Backlight();
        ~Backlight();

        void setBrightness(int target);

    private:
        std::fstream blb;
        int current_brightness;
};

#endif // BACKLIGHT_H

