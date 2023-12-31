
 #include "synth.h"

#include "../../LIBRARY/makra_mk.h"
#include "I2C_TWI/i2c_twi.h"




const uint16_t sin_wav[] = { 1800, 1831, 1862, 1894, 1925, 1956, 1988, 2019, 2050, 2081, 2112, 2143, 2174, 2204, 2235, 2265, 2296, 2326, 2356, 2386, 2415, 2445, 2474, 2503, 2532, 2560, 2589, 2617, 2645, 2672, 2699, 2727, 2753, 2780, 2806, 2832, 2857, 2883, 2908, 2932, 2956, 2980, 3004, 3027, 3050, 3072, 3094, 3116, 3137, 3158, 3178, 3198, 3218, 3237, 3256, 3274, 3292, 3309, 3326, 3342, 3358, 3374, 3389, 3403, 3417, 3431, 3444, 3456, 3468, 3480, 3491, 3501, 3511, 3521, 3530, 3538, 3546, 3553, 3560, 3566, 3572, 3577, 3582, 3586, 3590, 3593, 3595, 3597, 3598, 3599, 3599, 3599, 3598, 3597, 3595, 3593, 3590, 3586, 3582, 3577, 3572, 3566, 3560, 3553, 3546, 3538, 3530, 3521, 3511, 3501, 3491, 3480, 3468, 3456, 3444, 3431, 3417, 3403, 3389, 3374, 3358, 3342, 3326, 3309, 3292, 3274, 3256, 3237, 3218, 3198, 3178, 3158, 3137, 3116, 3094, 3072, 3050, 3027, 3004, 2981, 2957, 2932, 2908, 2883, 2858, 2832, 2806, 2780, 2753, 2727, 2700, 2672, 2645, 2617, 2589, 2560, 2532, 2503, 2474, 2445, 2415, 2386, 2356, 2326, 2296, 2266, 2235, 2205, 2174, 2143, 2112, 2081, 2050, 2019, 1988, 1957, 1925, 1894, 1862, 1831, 1800, 1768, 1737, 1705, 1674, 1643, 1612, 1580, 1549, 1518, 1487, 1456, 1425, 1395, 1364, 1334, 1304, 1273, 1243, 1214, 1184, 1155, 1125, 1096, 1068, 1039, 1011, 982, 955, 927, 900, 873, 846, 819, 793, 767, 742, 716, 691, 667, 643, 619, 595, 572, 549, 527, 505, 483, 462, 441, 421, 401, 381, 362, 343, 325, 307, 290, 273, 257, 241, 225, 210, 196, 182, 168, 155, 143, 131, 119, 108, 98, 88, 78, 69, 61, 53, 46, 39, 33, 27, 22, 17, 13, 9, 6, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 9, 13, 17, 22, 27, 33, 39, 46, 53, 61, 69, 78, 88, 97, 108, 119, 130, 142, 155, 168, 182, 196, 210, 225, 241, 256, 273, 290, 307, 325, 343, 362, 381, 400, 420, 441, 462, 483, 504, 527, 549, 572, 595, 618, 642, 666, 691, 716, 741, 767, 793, 819, 845, 872, 899, 927, 954, 982, 1010, 1039, 1067, 1096, 1125, 1154, 1184, 1213, 1243, 1273, 1303, 1333, 1364, 1394, 1425, 1456, 1487, 1518, 1549, 1580, 1611, 1642, 1674, 1705, 1736, 1768,

};

int main(void)
{
    // I2C pins as output
    DDRC |= ((1 << PC4) | (1 << PC5) );

    // test pin
    DDRB |= (1 << PB0);

    // All outputs as 0;
    PORTC = 0;

    i2cSetBitrate( bitratekHz );
    dpInit();

// SAMPLING INTERRUPT TIMER ////////////////////////////////////////////

    // Timer 1 CTC, TOP OCR1A, prescaler 1
    TCCR1A |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    // clock 20Mhz frequency of compare match 44100kHz
    TCNT1 = 0;          // IS IT NECESERRY???????????
    OCR1A = 225;
    // interrupt on compare match
    TIMSK1 |= (1 << OCIE1A);

// SAMPLING INTERRUPT TIMER ////////////////////////////////////////////

    sei();
    uint8_t intSwitch = 1;
    while(1)
    {

        rbyte = dpReadByte(0);

        // rbyte values:
        // 0 - no note, stop sampling interrupts 1 - C, 2 - D, 3 - E, ... , 8 - C2

        frqDiv = frqDivs[rbyte];
        frq1 = frq1s[rbyte];
        frq2 = frq1 + 1;


        // stop sampling interrupts if rbyte = 0, reset noteStart so when interrupts starts i2c communication can properly occur
        if( !rbyte && !intSwitch ){
            TIMSK1 &= ~(1 << OCIE1A);
            intSwitch = 1;
            noteStart = 0;
        }
        else if( rbyte && intSwitch ){
            PORTB ^= (1 << PB0);
            TIMSK1 |= (1 << OCIE1A);
            intSwitch = 0;
        }

    }

    return 0;
}


// interrupt vector
ISR (TIMER1_COMPA_vect)
{
    //PORTB |= (1<< PB0);

    static uint16_t waveCount = 0;
    if(waveCount >= 360)
        waveCount = 0;

    TCNT1 = 0; // NECESSERY??????????????

    int sample = sin_wav[waveCount];

// DO POPRAWY??
    if(!noteStart){
        TWI_start();
        TWI_write(0xC0);
        noteStart = 1;
    }

    TWI_write((uint16_t)(sample) >> 8);
    TWI_write((uint8_t)sample);
    //TWI_stop();

   static uint8_t freqTimer = 0;

    if(freqTimer < frqDiv)
        waveCount+=frq1;
    else
        waveCount+=frq2;

    freqTimer++;
    if(freqTimer == 10)
        freqTimer = 0;

    //PORTB &= ~(1<< PB0);
    //PINB |= (1 << PB0);

}

/*
ISR (TIMER0_COMPA_vect)
{
    //TCNT1 = 0;
    PINB |= (1 << PB0);
}
*/
