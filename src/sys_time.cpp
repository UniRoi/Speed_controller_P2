#include <sys_time.h>

// #include <avr/interrupt.h>
#include <avr/io.h>

sys_time::sys_time(/* args */)
{
    u32SysTimeCnt = 0;
}

sys_time::~sys_time()
{
}

void sys_time::init(void)
{
    const uint16_t period_ms = 1;
    /* Timer2 for timebase */
    TCCR2A |= (1 << WGM21); // Set the Timer Mode to CTC
     
    OCR2A = ((((16000000 * period_ms)/ 1000) / 64) - 1);  // Set the value that you want to count to

    TIMSK2 |= (1 << OCIE2A);    // Set the ISR COMPA vect

    TCCR2B |= (1 << CS22);      // set prescaler to 64 and start the timer
}

uint32_t sys_time::Get_SysTimeMs(void)
{
    return u32SysTimeCnt;
}

void sys_time::Inc_SysTimeMs(void)
{
    u32SysTimeCnt++;
}


