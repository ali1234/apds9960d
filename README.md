
A daemon providing gesture input and backlight brightness control.

Uses i2c-dev to read the sensor, sysfs gpio to receive interrupts,
sysfs backlight for brightness control, and uinput to synthesize
keyboard events.

Gestures are converted to up/down/left/right arrow key events.

Backlight brightness will be smooth scaled according to detected
ambient light level.
