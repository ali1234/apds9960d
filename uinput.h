#ifndef UINPUT_H
#define UINPUT_H

class Uinput
{
    public:
        Uinput();
        ~Uinput();

        void sendKey(int key);

    private:
        int fd;
};

#endif // UINPUT_H

