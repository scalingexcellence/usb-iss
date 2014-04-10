#ifndef USBISS_H
#define USBISS_H

namespace usbiss {

enum IssMode {
    IO_MODE = 0x00,
    IO_CHANGE = 0x10,
    I2C_S_20KHZ = 0x20,
    I2C_S_50KHZ = 0x30,
    I2C_S_100KHZ = 0x40, 
    I2C_S_400KHZ = 0x50,
    I2C_H_100KHZ = 0x60,
    I2C_H_400KHZ = 0x70,
    I2C_H_1000KHZ = 0x80,
    SPI_MODE = 0x90,
    SPI_MODE_TXFROMACTIVE_IDLE_HIGH = 0x91,
    SPI_MODE_TXFROMIDLE_IDLE_LOW = 0x92,
    SPI_MODE_TXFROMIDLE_IDLE_HIGH = 0x93,
    SERIAL = 0x01,
    I2C_S_20KHZ_SERIAL = 0x21,
    I2C_S_50KHZ_SERIAL = 0x31,
    I2C_S_100KHZ_SERIAL = 0x41, 
    I2C_S_400KHZ_SERIAL = 0x51,
    I2C_H_100KHZ_SERIAL = 0x61,
    I2C_H_400KHZ_SERIAL = 0x71,
    I2C_H_1000KHZ_SERIAL = 0x81
};


enum PinMode {
    OutputLow = 0x00,
    OutputHigh = 0x01,
    DigitalInput = 0x02,
    AnalogueInput = 0x03
};

enum Pin {
    Pin1 = 1,
    Pin2 = 2,
    Pin3 = 3,
    Pin4 = 4
};


enum Baud {
    BAUD300    = 0x270F,
    BAUD1200   = 0x09C3,
    BAUD2400   = 0x04E1,
    BAUD9600   = 0x0137,
    BAUD19_2k  = 0x009B,
    BAUD38_4k  = 0x004D,
    BAUD57_6k  = 0x0033,
    BAUD115_2k = 0x0019,
    BAUD250k   = 0x000B,
    BAUD1M     = 0x0003  
};

class UsbIss {
    int fd;
    char lastError[128];
    char serial[9];
    char modeName[30];
    
    void setModeName(unsigned char c);
    
public:
    
    UsbIss(const char * tty);
    
    void wr(unsigned char c);
    void wr(unsigned char c1, unsigned char c2);
    void wr(unsigned char c1, unsigned char c2, unsigned char c3);

    unsigned char rd();
    
    unsigned char getMode();
    
    const char * getModeStr();
    
    static unsigned char getIoType(PinMode p4, PinMode p3, PinMode p2, PinMode p1);
    
    /*
    For a 3Mhz SCK, divisor  =  (6000000/3000000)-1 =  1
    For 500khz SCK, divisor =   (6000000/ 500000)-1 = 11
    */
    static unsigned char getSckDivisor(unsigned int SCK);
    
    void setMode(unsigned char mode, unsigned char ioType_or_sck_divisor = 0x00, unsigned int baud = 0x0000);
    
    int setModeX(unsigned char mode, unsigned char ioType_or_sck_divisor = 0x00, unsigned int baud = 0x0000);
    
    int setPins(unsigned char val);
    
    int readPins();
    
    int readAnalog(Pin pin);
    
    char * getSerialNumber();
    
    static void printByte(unsigned char c);
    
    const char * getLastError() const;
};

} /* namespace usbiss */

#endif
