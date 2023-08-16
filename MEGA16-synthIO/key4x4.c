#include "key4x4.h"

void keyboardInit()
{
    // rows as outputs
	DDR(KEY_ROW1PORT) |= (1 << KEY_ROW1);
	DDR(KEY_ROW2PORT) |= (1 << KEY_ROW2);
	DDR(KEY_ROW3PORT) |= (1 << KEY_ROW3);
	DDR(KEY_ROW4PORT) |= (1 << KEY_ROW4);

    // cols as inputs
	DDR(KEY_COL1PORT) &= ~(1 << KEY_COL1);
	DDR(KEY_COL2PORT) &= ~(1 << KEY_COL2);
	DDR(KEY_COL3PORT) &= ~(1 << KEY_COL3);
	DDR(KEY_COL4PORT) &= ~(1 << KEY_COL4);
}

uint8_t keyboardState(){

    // counts debouncing cycles
    static uint8_t debouncingTimer = 0;
    static uint8_t debouncingTimer2 = 0;
    static uint8_t debouncingTimer3 = 0;
    static uint8_t debouncingTimer4 = 0;

    // alternate between checking 4 rows
    static uint8_t rowTimer = 0;
    static uint8_t rowNumber = 1;

    static uint8_t buttonPressed = 0;

// alternate between each row after 256 cycles /////////////////////////////////////////
    if(rowTimer == 0){
        rowNumber++;

        if(rowNumber == 4)
            rowNumber = 0;

        // set all rows to low
        PORT(KEY_ROW1PORT) &= ~(1 << KEY_ROW1);
        PORT(KEY_ROW2PORT) &= ~(1 << KEY_ROW2);
        PORT(KEY_ROW3PORT) &= ~(1 << KEY_ROW3);
        PORT(KEY_ROW4PORT) &= ~(1 << KEY_ROW4);

        // set current row to high
        switch(rowNumber){
        case 0:
            PORT(KEY_ROW1PORT) |= (1 << KEY_ROW1);
            break;
        case 1:
            PORT(KEY_ROW2PORT) |= (1 << KEY_ROW2);
            break;
        case 2:
            PORT(KEY_ROW3PORT) |= (1 << KEY_ROW3);
            break;
        case 3:
            PORT(KEY_ROW4PORT) |= (1 << KEY_ROW4);
            break;
        }
    }
    rowTimer++;
//////////////////////////////////////////////////////////////////////////////////////


// check wchich column is high. That in combination with current row set to high gets us current button pressed
    if( !debouncingTimer && (PIN(KEY_COL1PORT) & (1 << KEY_COL1)) ){
        debouncingTimer = 1;
        // this happens only once when button is pressed
        buttonPressed = 1 + (rowNumber * 4);
        //return buttonPressed;

    }else if(debouncingTimer && !(PIN(KEY_COL1PORT) & (1 << KEY_COL1))) debouncingTimer++; // debouncing happens here(max 256 cycles)
    // if button is still released after debouncing and checking all 4 rows is complete, reset pressedButton
    else if( !debouncingTimer && !(PIN(KEY_COL1PORT) & (1 << KEY_COL1)) && (buttonPressed == ((rowNumber * 4) + 1 )) ) buttonPressed = 0;


    if( !debouncingTimer2 && (PIN(KEY_COL2PORT) & (1 << KEY_COL2)) ){
        debouncingTimer2 = 1;

        buttonPressed = 2 + (rowNumber * 4);
        //return buttonPressed;

    } else if(debouncingTimer2 && !(PIN(KEY_COL2PORT) & (1 << KEY_COL2))) debouncingTimer2++;
    else if( !debouncingTimer2 && !(PIN(KEY_COL2PORT) & (1 << KEY_COL2)) && (buttonPressed == ((rowNumber * 4) + 2 )) ) buttonPressed = 0;

    if( !debouncingTimer3 && (PIN(KEY_COL3PORT) & (1 << KEY_COL3)) ){
        debouncingTimer3 = 1;

        buttonPressed = 3 + (rowNumber * 4);
        //return buttonPressed;

    } else if(debouncingTimer3 && !(PIN(KEY_COL3PORT) & (1 << KEY_COL3))) debouncingTimer3++;
    else if( !debouncingTimer3 && !(PIN(KEY_COL3PORT) & (1 << KEY_COL3)) && (buttonPressed == ((rowNumber * 4) + 3 )) ) buttonPressed = 0;

    if( !debouncingTimer4 && (PIN(KEY_COL4PORT) & (1 << KEY_COL4)) ){
        debouncingTimer4 = 1;

        buttonPressed = 4 + (rowNumber * 4);
        //lcd_cls();
        //lcd_int(buttonPressed);
        //return buttonPressed;

    } else if(debouncingTimer4 && !(PIN(KEY_COL4PORT) & (1 << KEY_COL4))) debouncingTimer4++;
    else if( !debouncingTimer4 && !(PIN(KEY_COL4PORT) & (1 << KEY_COL4)) && (buttonPressed == ((rowNumber * 4) + 4 )) ) buttonPressed = 0;

    return buttonPressed;
}
