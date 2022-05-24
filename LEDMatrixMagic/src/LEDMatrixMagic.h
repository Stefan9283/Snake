/*
 *    LEDMatrixMagic.h - A library for controling Leds with a MAX7219
*/

#ifndef LedControl_h
#define LedControl_h

#include <Arduino.h>

class LEDMatrixMagic {
    private:
        int DIN;
        int CLK;
        int CS;
        int maxDevices;
        // a buffer used in spi_transfer
        byte spidata[16];
        // an array for for each matrix
        // every array holds a byte containing 
        // the state of all of the LEDs on line 'i'
        byte status[8][8]; 

        // byte *message_buffer[8];
        // int current_left_col = -1;
    public:
       
        LEDMatrixMagic(int dataPin, int clkPin, int csPin, int numDevices=1);
        int matrix_count();
        void set_intensity(uint8_t addr, uint8_t value);
        void clear(int addr);
        void shutdown(int addr, bool b);
        void set_scan_limit(int addr, int limit);
        void set_led(int addr, int row, int column, boolean state);
        void set_row(int addr, int row, uint8_t state);
        // void set_message(char *message);
        void spi_transfer(uint8_t address, uint8_t opcode, uint8_t data);
};

#endif



