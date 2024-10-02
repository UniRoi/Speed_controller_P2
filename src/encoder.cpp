#include <encoder.h>
#include <avr/interrupt.h>
#include <avr/io.h>


// #include <digital_out.h>
// Digital_out Led(5);

static volatile uint16_t ui16TimerCnt = 0;
static volatile bool bTimerFlag = false;

encoder::encoder(int8_t pin_A_number, int8_t pin_B_number): EncoderAPin(pin_A_number) , EncoderBPin(pin_B_number)
{

}

encoder::~encoder()
{
}

void encoder::init(void)
{
    // Led.init();

    // use external interrupt 0, to which the encoder pin A is connected
    EICRA = (1 << ISC01) | (1 << ISC00); // The rising edge of INT0 generates an interrupt request.
    EIMSK = (1 << INT0); // External Interrupt Request 0 Enable

    const uint16_t period_ms = 2;
    /* Timer0 for timebase */
    TCCR0A |= (1 << WGM01); // Set the Timer Mode to CTC
     
    OCR0A = ((((16000000 * period_ms)/ 1000) / 256) - 1);  // Set the value that you want to count to

    TIMSK0 |= (1 << OCIE0A);    // Set the ISR COMPA vect

    TCCR0B |= (1 << CS02);      // set prescaler to 256 and start the timer
}



void encoder::updatePos(void)
{
    bool bDirection = false;

    // state change occured 
    bDirection = EncoderBPin.is_hi();
    
    // rising edge detected
    if(bDirection == true)
    {
        /* negative direction */ 
        i16EncoderPos--;
    }
    else
    {
        /* positive direction */ 
        i16EncoderPos++;
    }

}

// static int16_t getPosition(void)
// {
//     return i16EncoderPos;
// }

int16_t encoder::GetPosition(void)
{
    return i16EncoderPos;
}

void encoder::ResetPosition(void)
{
    EIMSK = 0;//(0 << INT0); // Disable Interrupt Request 0 Enable

    /* set encoder pos to zero for easier calculation */
    i16EncoderPos = 0;

    EIMSK = (1 << INT0); // External Interrupt Request 0 Enable

}


void encoder::updatePps(void)
{
    // static int16_t lastEncPos = 0;
    // int16_t EncPos = 0;
    

    // if(bTimerFlag == true)
    {
        bTimerFlag = false;
        /* Calculate Rps with timebase of expired timer */
        i16MotorPps = GetPosition();
        ResetPosition();

        //  = (EncPos * 20);
    }
}


int16_t encoder::GetPps(void)
{
    return i16MotorPps * 50;
}

int16_t encoder::GetRpm(void)
{
    return static_cast<int16_t>((GetPps() / 7.0) *60);
}


    
