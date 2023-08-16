#include <avr/io.h>
#include "../../LIBRARY/makra_mk.h"
#include "LCD/lcd44780.h"

// config ports to wchich keyboard is connected
#define KEY_ROW1PORT  B
#define KEY_ROW1 0
#define KEY_ROW2PORT  B
#define KEY_ROW2 1
#define KEY_ROW3PORT  B
#define KEY_ROW3 2
#define KEY_ROW4PORT  B
#define KEY_ROW4 3

#define KEY_COL1PORT  D
#define KEY_COL1 0
#define KEY_COL2PORT  D
#define KEY_COL2 1
#define KEY_COL3PORT  D
#define KEY_COL3 3
#define KEY_COL4PORT  D
#define KEY_COL4 4

void keyboardInit();
uint8_t keyboardState();
