
#include "synth.h"

const uint16_t frequencies[FRQ_SIZE] = {523.0, 587.0, 659.0, 698.0, 784.0, 880.0, 987.0, 1047.0};

const uint8_t frqDivs[17] = {0, 7, 5, 2, 9, 6, 3, 0, 6, 2, 8, 4, 9, 5, 0, 4, 9};
const uint8_t frq1s[17] =   {0, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 7, 8, 8, 8, 9, 10};

volatile uint8_t byFrame[BYFRAME_SIZE] = {0};

// actually its around 600-780kHz
const uint16_t bitratekHz = 800;


void dpInit()
{
    // INTERRUPT line as output
    DDR(INT_PORT) |= (1 << INT);

    // CLOCK line as output
    DDR(CLK_PORT) |= (1 << CLK);

    // DATA line as input
    DDR(SDA_PORT) &= ~(1 << SDA);

    noteStart = 0;
    brFlag = 0;
    spFlag = 0;


}

void timersSetup()
{
// SAMPLING INTERRUPT TIMER ////////////////////////////////////////////

    // Timer1 CTC, TOP OCR1A, prescaler 1
    TCCR1A |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    // AVRclock=20Mhz frequency of compare match 44100kHz
    TCNT1 = 0;          // IS IT NECESERRY???????????
    OCR1A = 225;
    // interrupt on compare match
    TIMSK1 |= (1 << OCIE1A);

// SAMPLING INTERRUPT TIMER ////////////////////////////////////////////

// DATA PROTOCOL TIMER ////////////////////////////////////////////////

    // Timer0 CTC, TOP OCR0A, prescaler 1
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS00);
    // frequency 45kHz
    TCNT0 = 0;
    OCR0A = 200;

    //TIMSK0 |= (1 << OCIE0A);

// DATA PROTOCOL TIMER ///////////////////////////////////////////////
}

void portSetup()
{
    // I2C pins as output
    DDRC |= ((1 << PC4) | (1 << PC5) );

    // test pin
    DDRB |= (1 << PB0);

    // All outputs as 0;
    PORTC = 0;
}


uint8_t dpReadByte(uint8_t sw)
{
    static uint8_t byte = 0;
    static uint8_t pow = 1;
    static uint8_t bitsRead = 0;

    // if whole byte was read, set new interrupt
    if( !bitsRead && !sw )
    {
        byte = 0;
        pow = 1;
        brFlag = 0;

        // INTERRUPT high
        PORT(INT_PORT) |= (1 << INT);

        // INTERRUPT low
        if( !noteStart )
            _delay_us(5);
        PORT(INT_PORT) &= ~(1 << INT);
    }

// sw switched in Timer0 interrupt alternately makes function behave like this:
// 1. set dp clock high
// 2. wait 1 timer0 cycle
// 3. read bit and set dp clock low
// 4. rinse repeat

    if( !sw || noteStart )
    {
        // set clock high
        PORT(CLK_PORT) |= (1 << CLK);

    }

    if( sw || noteStart )
    {
        // read state of SDA pin, convert it to 0/1 value and mulitply by significance in byte
        // little endian
        if( (PIN(SDA_PORT) & (1 << SDA)) ){
            byte += 1 * pow;
        }

        pow = pow * 2;

        PORT(CLK_PORT) &= ~(1 << CLK);

        bitsRead++;
        if(bitsRead == 8){
            bitsRead = 0;
            brFlag = 1;
        }
    }

    return byte;
}

void dpReadFrame(uint8_t sw)
{
    static uint8_t byte = 0;
    static uint8_t br = 0;

    static uint8_t buffer[BYFRAME_SIZE] = {0};

    byte = dpReadByte(sw);

    // if whole byte was read;
    if( brFlag ){
        // put byte into buffer;
        buffer[br] = byte;
        br++;

        // if all bytes of 1 frame was read, copy from buffer to byFrame
        if( br == BYFRAME_SIZE ){
            br = 0;
            uint8_t i;
            for(i = 0; i < BYFRAME_SIZE; i++){
                byFrame[i] = buffer[i];
            }

        rbyte = byFrame[0];

        }
    }
}
