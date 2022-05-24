/*
 *    A library for controling Leds with a MAX7219
 */

#include "LEDMatrixMagic.h"
#include <SPI.h>

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

LEDMatrixMagic::LEDMatrixMagic(int dataPin, int clkPin, int csPin, int numDevices) {
    DIN = dataPin;
    CLK = clkPin;
    CS = csPin;
    if(numDevices <= 0 || numDevices > 8 )
        numDevices = 8;
    maxDevices=numDevices;
    pinMode(DIN, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(CS,  OUTPUT);
    digitalWrite(CS, HIGH);
    // SPI.setBitOrder(MSBFIRST);
    // SPI.begin();
    for (int i = 0; i < 8; i++)
        memset(status[i], 0, 8);
    for(int i = 0; i < maxDevices; i++) {
        spi_transfer(i, OP_DISPLAYTEST, 0);
        //scanlimit is set to max on startup
        set_scan_limit(i, 7);
        //decode is done in source
        spi_transfer(i, OP_DECODEMODE,0);
        clear(i);
        //we go into shutdown-mode on startup
        shutdown(i, true);
    }

}

int LEDMatrixMagic::matrix_count() {
    return maxDevices;
}

void LEDMatrixMagic::shutdown(int addr, bool b) {
    if(addr < 0 || addr >= maxDevices)
        return;

    if(b)
        spi_transfer(addr, OP_SHUTDOWN,0);
    else
        spi_transfer(addr, OP_SHUTDOWN,1);
}

void LEDMatrixMagic::set_scan_limit(int addr, int limit) {
    if(addr < 0 || addr >= maxDevices)
        return;
    if(limit >= 0 && limit < 8)
        spi_transfer(addr, OP_SCANLIMIT, limit);
}

void LEDMatrixMagic::set_intensity(uint8_t addr, uint8_t value) {
    if(addr < 0 || addr >= maxDevices)
        return;
    spi_transfer(addr, OP_INTENSITY, value);
}

void LEDMatrixMagic::clear(int addr) {
    if(addr<0 || addr>=maxDevices)
        return;
    
    int offset = addr * 8;

    for(int i = 0; i < 8; i++) {
        status[addr][i] = 0;
        spi_transfer(addr, i+1, 0);
    }
}

void LEDMatrixMagic::set_led(int addr, int row, int column, boolean state) {
    if(addr < 0 || addr >= maxDevices)
        return;

    if(row<0 || row>7 || column<0 || column>7)
        return;

    byte val = 0x00;
    val=B10000000 >> column;

    if(state)
        status[addr][row]=status[addr][row] | val;
    else {
        val =~ val;
        status[addr][row]=status[addr][row] & val;
    }
    spi_transfer(addr, row + 1,status[addr][row]);
}
void LEDMatrixMagic::set_row(int addr, int row, uint8_t state) {
    status[addr][row] = state;
    spi_transfer(addr, row + 1,status[addr][row]);
}
// void LEDMatrixMagic::set_message(char *message) {
//     this->current_left_col = 0;
    
//     Serial.print(message);
//     Serial.print(' ');
//     Serial.println(strlen(message));
    
//     for (int i = 0; i < 8; i++) 
//         message_buffer[i] = calloc(8 * strlen(message), sizeof(char));

//     for (int l = 0; l < strlen(message); l++) {
//         for (int i = 0; i < 8; i++) { 
//             byte row = font[message[l] - 32];
//             for (int j = 7; j >= 0; j++) {
//                 message_buffer[i][j + l * 8] &= (row & 1);
//                 row = row >> 1; 
//             }
//         }
//     }

//     Serial.println(message);
//     for (int i = 0; i < 8; i++) { 
//         for (int j = 0; j < strlen(message) * 8; j++) {
//             if (message_buffer[i][j])
//                 Serial.print("#");
//             else 
//                 Serial.print(' ');
//         }
//         Serial.print('\n');
//     }

// }

void LEDMatrixMagic::spi_transfer(uint8_t addr, uint8_t opcode, uint8_t data) {
    //Create an array with the data to shift out
    int offset = addr * 2;

    memset(spidata, 0, 16);

    //put our device data into the array
    spidata[offset + 1] = opcode;
    spidata[offset] = data;
    //enable the line 
    digitalWrite(CS, LOW);

    //Now shift out the data 
    int maxbytes = maxDevices * 2;
    for(int i = maxbytes - 1; i >= 0; i--)
        // SPI.transfer(spidata[i]);
        shiftOut(DIN, CLK, MSBFIRST, spidata[i]);
    //latch the data onto the display
    digitalWrite(CS, HIGH);
}



