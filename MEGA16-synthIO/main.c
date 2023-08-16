#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "key4x4.h"

#define INT_PORT D
#define INT 2
#define CLK_PORT D
#define CLK 7
#define SDA_PORT_ D
#define SDA 6

#define BYFRAME_SIZE 3

enum action{kReleased, kPressed, knob1, knob2, knob3};

volatile uint8_t keyPressed = 0;

// every time a key is pressed or knob position is changed, 3 bytes will be sent
// 1st byte - ID of action that happend
// 2nd byte - least important byte of information
// 3rd byte - most important byte of information

uint8_t byFrame[BYFRAME_SIZE] = {1, 1, 1};

void sendFrame();

volatile uint8_t frameCounter = 0;

int main(void)
{
    keyboardInit();
    lcd_init();
    sei();

    uint8_t keyState = 0;

    while(1){
        keyState = keyboardState();
        if(keyState != keyPressed){
            keyPressed = keyState;
            lcd_cls();

            if( keyState == 0 )
                byFrame[0] = kReleased;
            else
                byFrame[0] = kPressed;


            byFrame[1] = keyPressed;
            byFrame[2] = 0;
            lcd_cls();
            lcd_int(byFrame[0]);
            lcd_str("  ");
            lcd_int(byFrame[1]);
            lcd_str("  ");
            lcd_int(byFrame[2]);
            lcd_str("  ");
        }

    }
}


void sendFrame()
{


}

