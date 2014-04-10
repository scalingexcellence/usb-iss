#include <iostream>
#include <unistd.h>  /* UNIX standard function definitions */

#include "usbiss.h"

using namespace std;
using namespace usbiss;

void tryIo(UsbIss & s) {
    s.setMode(IO_MODE, UsbIss::getIoType(AnalogueInput, AnalogueInput, AnalogueInput, AnalogueInput));
    for (int i = 0; i < 100; i++) {
        for (int i = 1; i <= 4; i ++) {
            cout << "pin " << i << ": " << s.readAnalog((Pin)i) << endl;
        }
        cout << endl;
        usleep(250000);
    }
}

int main (int argc, char * const argv[]) {
    try {
        UsbIss s("/dev/tty.usbmodem00010851");
    
        cout << "Serial: " << s.getSerialNumber() << std::endl;
    
        // ISS mode is initialized to 0x40 (I2C-S_100KHZ) on power up
        cout << "Mode: " << s.getModeStr() << endl;
        tryIo(s);
        cout << "Mode: " << s.getModeStr() << endl;
        s.setMode(I2C_S_100KHZ);
        cout << "Mode: " << s.getModeStr() << endl;
        s.setMode(I2C_H_100KHZ);
        cout << "Mode: " << s.getModeStr() << endl;
        s.setMode(SERIAL);
        cout << "Mode: " << s.getModeStr() << endl;
    }
    catch (const char * err) {
        cerr << "Fatal: " << err << endl;
    }
    
    return 0;
}
