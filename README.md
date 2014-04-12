usb-iss
=======

Arduino compatible Mac/Linux drivers for Usb-Iss module (http://www.robot-electronics.co.uk/htm/usb_iss_tech.htm).

This is a lib with which you can develop I2C/SPI etc. code quickly in C++ using your Mac or PC (Linux) and USB-ISS and then be able to run it on Arduino with minor changes.

The way to use is very simple, just...

    make

Then you can run the examples with:

	build/leds <path to your terminal tty device (/dev/tty...)>
    build/modes_and_analog <path to your terminal tty device (/dev/tty...)>

The program right now reads some analog values from the pins configured as Analog Inputs. The infrastructure can easily support/implements tons of further improvements.

Todo:

- [ ] Put a timeout on the reads/writes (they occasionally fail)
- [ ] Crete the I2C interface for the MPU6050 gyroscope
- [ ] Document the server / leds example
