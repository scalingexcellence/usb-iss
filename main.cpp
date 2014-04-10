/*
  main.cpp - USB-ISS C++ interface
  Copyright (c) 2014 Dimitrios Kouzis-Loukas.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

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
    const char * ttyPath = "/dev/tty.usbmodem00010851";
    if (argc > 2) {
        cerr << "Unexpected number of parameters" << endl;
    }
    else if (argc == 2) {
      ttyPath = argv[1];
    }
    try {
        UsbIss s(ttyPath);
    
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
