#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iomanip>
#include <cassert>

#include "usbiss.h"

using namespace std;

namespace usbiss {

// See also...
// http://galindezrobotics.blogspot.co.uk/2012/01/installing-usb-iss-driver-on-linux-and.html

// Anonymous namespace with local functions
namespace {
    // Covers all the I2C modes above without serial set
    int isI2CnotSerial(unsigned char c) {
        return ((c & 0x0f) == 0u) && (((c >> 4) & 0x0f) >= 2u) && (((c >> 4) & 0x0f) <= 8u);
    }

    // Covers all the I2C modes above with serial set
    int isI2CAndSerial(unsigned char c) {
        return ((c & 0x0f) == 1u) && (((c >> 4) & 0x0f) >= 2u) && (((c >> 4) & 0x0f) <= 8u);
    }

    int isSpi(unsigned char c) {
        return (c == SPI_MODE)
            ||  (c == SPI_MODE_TXFROMACTIVE_IDLE_HIGH)
            ||  (c == SPI_MODE_TXFROMIDLE_IDLE_LOW)
            ||  (c == SPI_MODE_TXFROMIDLE_IDLE_HIGH);
    }


    void testisI2Cs() {
        printf("IO_MODE: %d %d %d\n", isI2CnotSerial(IO_MODE), isI2CAndSerial(IO_MODE), isSpi(IO_MODE));
        printf("IO_CHANGE: %d %d %d\n", isI2CnotSerial(IO_CHANGE), isI2CAndSerial(IO_CHANGE), isSpi(IO_CHANGE));
        printf("I2C_S_20KHZ: %d %d %d\n", isI2CnotSerial(I2C_S_20KHZ), isI2CAndSerial(I2C_S_20KHZ), isSpi(I2C_S_20KHZ));
        printf("I2C_S_50KHZ: %d %d %d\n", isI2CnotSerial(I2C_S_50KHZ), isI2CAndSerial(I2C_S_50KHZ), isSpi(I2C_S_50KHZ));
        printf("I2C_S_100KHZ: %d %d %d\n", isI2CnotSerial(I2C_S_100KHZ), isI2CAndSerial(I2C_S_100KHZ), isSpi(I2C_S_100KHZ));
        printf("I2C_S_400KHZ: %d %d %d\n", isI2CnotSerial(I2C_S_400KHZ), isI2CAndSerial(I2C_S_400KHZ), isSpi(I2C_S_400KHZ));
        printf("I2C_H_100KHZ: %d %d %d\n", isI2CnotSerial(I2C_H_100KHZ), isI2CAndSerial(I2C_H_100KHZ), isSpi(I2C_H_100KHZ));
        printf("I2C_H_400KHZ: %d %d %d\n", isI2CnotSerial(I2C_H_400KHZ), isI2CAndSerial(I2C_H_400KHZ), isSpi(I2C_H_400KHZ));
        printf("I2C_H_1000KHZ: %d %d %d\n", isI2CnotSerial(I2C_H_1000KHZ), isI2CAndSerial(I2C_H_1000KHZ), isSpi(I2C_H_1000KHZ));
        printf("SPI_MODE: %d %d %d\n", isI2CnotSerial(SPI_MODE), isI2CAndSerial(SPI_MODE), isSpi(SPI_MODE));
        printf("SPI_MODE_TXFROMACTIVE_IDLE_HIGH: %d %d %d\n", isI2CnotSerial(SPI_MODE_TXFROMACTIVE_IDLE_HIGH), isI2CAndSerial(SPI_MODE_TXFROMACTIVE_IDLE_HIGH), isSpi(SPI_MODE_TXFROMACTIVE_IDLE_HIGH));
        printf("SPI_MODE_TXFROMIDLE_IDLE_LOW: %d %d %d\n", isI2CnotSerial(SPI_MODE_TXFROMIDLE_IDLE_LOW), isI2CAndSerial(SPI_MODE_TXFROMIDLE_IDLE_LOW), isSpi(SPI_MODE_TXFROMIDLE_IDLE_LOW));
        printf("SPI_MODE_TXFROMIDLE_IDLE_HIGH: %d %d %d\n", isI2CnotSerial(SPI_MODE_TXFROMIDLE_IDLE_HIGH), isI2CAndSerial(SPI_MODE_TXFROMIDLE_IDLE_HIGH), isSpi(SPI_MODE_TXFROMIDLE_IDLE_HIGH));
        printf("SERIAL: %d %d %d\n", isI2CnotSerial(SERIAL), isI2CAndSerial(SERIAL), isSpi(SERIAL));
        printf("I2C_S_20KHZ_SERIAL: %d %d %d\n", isI2CnotSerial(I2C_S_20KHZ_SERIAL), isI2CAndSerial(I2C_S_20KHZ_SERIAL), isSpi(I2C_S_20KHZ_SERIAL));
        printf("I2C_S_50KHZ_SERIAL: %d %d %d\n", isI2CnotSerial(I2C_S_50KHZ_SERIAL), isI2CAndSerial(I2C_S_50KHZ_SERIAL), isSpi(I2C_S_50KHZ_SERIAL));
        printf("I2C_S_100KHZ_SERIAL: %d %d %d\n", isI2CnotSerial(I2C_S_100KHZ_SERIAL), isI2CAndSerial(I2C_S_100KHZ_SERIAL), isSpi(I2C_S_100KHZ_SERIAL));
        printf("I2C_S_400KHZ_SERIAL: %d %d %d\n", isI2CnotSerial(I2C_S_400KHZ_SERIAL), isI2CAndSerial(I2C_S_400KHZ_SERIAL), isSpi(I2C_S_400KHZ_SERIAL));
        printf("I2C_H_100KHZ_SERIAL: %d %d %d\n", isI2CnotSerial(I2C_H_100KHZ_SERIAL), isI2CAndSerial(I2C_H_100KHZ_SERIAL), isSpi(I2C_H_100KHZ_SERIAL));
        printf("I2C_H_400KHZ_SERIAL: %d %d %d\n", isI2CnotSerial(I2C_H_400KHZ_SERIAL), isI2CAndSerial(I2C_H_400KHZ_SERIAL), isSpi(I2C_H_400KHZ_SERIAL));
        printf("I2C_H_1000KHZ_SERIAL: %d %d %d\n", isI2CnotSerial(I2C_H_1000KHZ_SERIAL), isI2CAndSerial(I2C_H_1000KHZ_SERIAL), isSpi(I2C_H_1000KHZ_SERIAL));
    }

    enum Commands {
        ISS_VERSION = 0x01,
        ISS_MODE = 0x02,
        GET_SER_NUM = 0x03,

        USB_ISS = 0x5A,
        SETPINS = 0x63,
        GETPINS = 0x64,
        GETAD = 0x65
    };

    enum Acks {
        ACK = 0xff,
        NACK = 0x00
    };
}

void UsbIss::setModeName(unsigned char c) {
    switch (c) {
        case IO_MODE:                         sprintf(modeName, "IO_MODE"); break;
        case IO_CHANGE:                       sprintf(modeName, "IO_CHANGE"); break;
        case I2C_S_20KHZ:                     sprintf(modeName, "I2C_S_20KHZ"); break;
        case I2C_S_50KHZ:                     sprintf(modeName, "I2C_S_50KHZ"); break;
        case I2C_S_100KHZ:                    sprintf(modeName, "I2C_S_100KHZ"); break;
        case I2C_S_400KHZ:                    sprintf(modeName, "I2C_S_400KHZ"); break;
        case I2C_H_100KHZ:                    sprintf(modeName, "I2C_H_100KHZ"); break;
        case I2C_H_400KHZ:                    sprintf(modeName, "I2C_H_400KHZ"); break;
        case I2C_H_1000KHZ:                   sprintf(modeName, "I2C_H_1000KHZ"); break;
        case SPI_MODE:                        sprintf(modeName, "SPI_MODE - Tx on transition from active to Idle clock, Idle state for clock is low level"); break;
        case SPI_MODE_TXFROMACTIVE_IDLE_HIGH: sprintf(modeName, "SPI - Tx on transition from active to Idle clock, Idle state for clock is high level"); break;
        case SPI_MODE_TXFROMIDLE_IDLE_LOW:    sprintf(modeName, "SPI - Tx on transition from Idle to active clock, Idle state for clock is low level"); break;
        case SPI_MODE_TXFROMIDLE_IDLE_HIGH:   sprintf(modeName, "SPI - Tx on transition from Idle to active clock, Idle state for clock is high level"); break;
        case SERIAL:                          sprintf(modeName, "IO_MODE+SERIAL"); break;
        case I2C_S_20KHZ_SERIAL:              sprintf(modeName, "I2C_S_20KHZ+SERIAL"); break;
        case I2C_S_50KHZ_SERIAL:              sprintf(modeName, "I2C_S_50KHZ+SERIAL"); break;
        case I2C_S_100KHZ_SERIAL:             sprintf(modeName, "I2C_S_100KHZ+SERIAL"); break;
        case I2C_S_400KHZ_SERIAL:             sprintf(modeName, "I2C_S_400KHZ+SERIAL"); break;
        case I2C_H_100KHZ_SERIAL:             sprintf(modeName, "I2C_H_100KHZ+SERIAL"); break;
        case I2C_H_400KHZ_SERIAL:             sprintf(modeName, "I2C_H_400KHZ+SERIAL"); break;
        case I2C_H_1000KHZ_SERIAL:            sprintf(modeName, "I2C_H_1000KHZ+SERIAL"); break;
        default: sprintf(modeName, "Unknown mode 0x%02x", c); break;
    }
}

UsbIss::UsbIss(const char * tty) {
    memset(lastError, 0, 128);
    
    fd = open(tty, O_RDWR | O_NOCTTY | O_NONBLOCK );
    if (fd == -1) {
        throw "Unable to open port";
    }

    struct termios tio;

    memset(&tio, 0, sizeof(tio));
    
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;
    
    cfsetospeed(&tio,B115200);            // 115200 baud
    cfsetispeed(&tio,B115200);            // 115200 baud
    
    tcsetattr(fd,TCSANOW,&tio);
    
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK); // Make the reads/writes blocking
}

void UsbIss::wr(unsigned char c) {
    if (write(fd, &c, 1)!=1) {
        throw "Write failed";
    }
}

void UsbIss::wr(unsigned char c1, unsigned char c2) {
    wr(c1);wr(c2);
}

void UsbIss::wr(unsigned char c1, unsigned char c2, unsigned char c3) {
    wr(c1);wr(c2);wr(c3);
}

unsigned char UsbIss::rd() {
    unsigned char c;
    if (read(fd, (void*)&c, 1)!=1) {
        throw "Read failed";
    }
    return c;
}    

unsigned char UsbIss::getMode() {
    wr(USB_ISS, ISS_VERSION);
    if (rd()!=7) {
        throw "Device is not a USB-ISS (Device Code 7)";
    }
    rd();
    return rd();
}

const char * UsbIss::getModeStr() {
    setModeName(getMode());
    return modeName;
}

unsigned char UsbIss::getIoType(PinMode p4, PinMode p3, PinMode p2, PinMode p1) {
    return ((unsigned char)p4) << 6 |
            ((unsigned char)p3) << 4 |
            ((unsigned char)p2) << 2 |
            ((unsigned char)p1);
}

/*
For a 3Mhz SCK, divisor  =  (6000000/3000000)-1 =  1
For 500khz SCK, divisor =   (6000000/ 500000)-1 = 11
*/
unsigned char UsbIss::getSckDivisor(unsigned int SCK) {
    return (6000000u/SCK) - 1u;
}

void UsbIss::setMode(unsigned char mode, unsigned char ioType_or_sck_divisor, unsigned int baud) {
    if (setModeX(mode, ioType_or_sck_divisor, baud)) {
        printf("%s\n", getLastError());
    }
}

int UsbIss::setModeX(unsigned char mode, unsigned char ioType_or_sck_divisor, unsigned int baud) {
    wr(USB_ISS, ISS_MODE, mode);
    
    if (mode == SERIAL || isI2CAndSerial(mode)) {
        // High and then Low byte
        wr((baud >> 8) & 0xff, baud & 0xff);
    }
    
    if (mode == IO_MODE || mode == SERIAL || mode == IO_CHANGE || isSpi(mode) || isI2CnotSerial(mode)) {
        wr(ioType_or_sck_divisor);
    }
    
    unsigned char r1 = rd(), r2 = rd();
    
    if (r1==ACK) { // ACKnowledge
        if (r2!=0x00) { // Panic mode
            sprintf(lastError, "Panic Ack 2nd response: %02x", r2);
            return -1;                
        }
    }
    else if (r1==NACK) { // NotACKnowledge
        if (r2==0x05) {
            strcpy(lastError, "Unknown Command");
            return -1;
        }
        else if (r2==0x06) {
            strcpy(lastError, "Internal Error 1");
            return -1;
        }
        else if (r2==0x06) {
            strcpy(lastError, "Internal Error 2");
            return -1;
        }
        else { // Panic mode
            sprintf(lastError, "Panic Nack 2nd response: %02x", r2);
            return -1;
        }
    }
    else { // Panic mode
        strcpy(lastError, "Panic 1st response");
        return -1;
    }
    return 0;
}

int UsbIss::setPins(unsigned char val) {
    wr(SETPINS, val);
    unsigned char rv = rd();
    if (rv != ACK) {
        sprintf(lastError, "SetPins Failed: %02x", rv);
        return -1;
    }
    return 0;
}

int UsbIss::readPins() {
    wr(GETPINS);
    return rd();
}

int UsbIss::readAnalog(Pin pin) {
    if (pin < 1 || pin > 4) {
        throw "Unexpected pin number";
    }
    wr(GETAD, pin);
    unsigned char high = rd(), low = rd();
    return (high << 8) | low;
}

char * UsbIss::getSerialNumber() {
    wr(USB_ISS, GET_SER_NUM);
    for (int i = 0; i < 8; ++i) {
        serial[i] = rd();
    }
    serial[8] = 0;
    return serial;
}

void UsbIss::printByte(unsigned char c) {
    printf("%02x\n",c);
}

const char * UsbIss::getLastError() const {
    return lastError;
}

} /* namespace usbiss */
