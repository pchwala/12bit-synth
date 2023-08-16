#ifndef SYNTH_H
#define SYNTH_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#include "../../LIBRARY/makra_mk.h"


#define BIT(x) (1<<x)

// config ports of data protocol
#define INT_PORT B
#define INT 1
#define CLK_PORT D
#define CLK 6
#define SDA_PORT B
#define SDA 2

#define BYFRAME_SIZE 3
#define FRQ_SIZE 9

volatile uint8_t rbyte;
volatile uint8_t noteStart;
volatile uint8_t frqDiv, frq1, frq2;

// byte read flag
volatile uint8_t brFlag;
// sample play flag
uint8_t spFlag;

extern const uint8_t frqSize;
extern const uint16_t frequencies[];

extern const uint8_t frqDivs[];
extern const uint8_t frq1s[];

extern volatile uint8_t byFrame[];

extern const uint16_t bitratekHz;

void dpInit();
void timersSetup();
void portSetup();
uint8_t dpReadByte(uint8_t sw);
void dpReadFrame(uint8_t sw);

#endif // SYNTH_H
