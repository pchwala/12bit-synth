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

volatile uint8_t keyPressed = 0;

uint8_t byFrame[BYFRAME_SIZE] = {1, 1, 1};

volatile uint8_t frameCounter = 0;

int main(void)
{
    keyboardInit();
    lcd_init();

    // enable interrupts on INT0 // rising edge
    GICR |= (1 << INT0);
    MCUCR |= ( (1 << ISC01) | (1 << ISC00) );

    sei();

    // pin SDA jako wyjscie
    DDR(SDA_PORT_) |= (1 << SDA);

    // pin CLK jako wejscie
    DDR(CLK_PORT) &= ~(1 << CLK);

    uint8_t keyState = 0;

    while(1){
        keyState = keyboardState();
        if(keyState != keyPressed){
            keyPressed = keyState;
            lcd_cls();
            lcd_int(keyPressed);



            byFrame[1] = keyPressed;
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


ISR (INT0_vect)
{
    uint8_t i;
    for(i = 0; i < 8; i++){

        // wait until clock rises
        while( !(PIN(CLK_PORT) & (1 << CLK)) );

        // send high or low bit of byte
        if( (keyPressed & (1 << i)) ){
            PORT(SDA_PORT_) |= (1 << SDA);
        }
        else
            PORT(SDA_PORT_) &= ~(1 << SDA);

        // wait until clock falls
        while( PIN(CLK_PORT) & (1 << CLK) );

    }

}

