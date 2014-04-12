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

#include "../usbiss.h"

using namespace std;
using namespace usbiss;

static const unsigned int gray[] = { 1, 3, 2, 6, 7, 5, 4, 12, 13, 15, 14, 10, 11, 9, 8 };

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
    
        s.setMode(IO_MODE, UsbIss::getIoType(OutputLow, OutputLow, OutputLow, OutputLow));
        
        while (true) {
            for (unsigned int i = 0; i < 16u; ++i) {
                s.setPins(gray[i]);
                usleep(250000);
            }
        }
    }
    catch (const char * err) {
        cerr << "Fatal: " << err << endl;
    }
    
    return 0;
}
