#include <encoder.h>
#include <util/delay.h>
#include <digital_out.h>
#include <Arduino.h>

#include <avr/interrupt.h>

static uint16_t lastEncCnt = 0;
static int16_t lastRps = 0;
encoder Encoder(2, 3);
Digital_out Led(4);
Digital_out Hbridge(1);

int main(void)
{
  uint32_t lastTime = 0;
  uint32_t currTime = 0;
  uint16_t ui16EncCnt = 0;
  int16_t i16Rps = 0;
  
  
  Led.init();
  Hbridge.init();

  // here interrupt registers are set
  Encoder.init();
  sei();

  // Add serial for part 2
  Serial.begin(115200);
    // delay(1000);
  
  // Hbridge.set_hi();
  // Led.toggle();
  
  int i = 0;
  while (1)
  {
    // i++;
    // if (i == 100)
    // {
      // Serial.print("Starting measurement\n");
      
      // lastTime = micros();
      
    // }
    /* infinity loop */
    _delay_ms(10);
    // Encoder.update();
    // Encoder.updatePps();
    i16Rps = Encoder.GetRpm();

    // if(ui16EncCnt != lastEncCnt)
    // {
      
    //   lastEncCnt = ui16EncCnt;

    //   // print counter to serial
    //   Serial.print(ui16EncCnt);
    //   Serial.println();
    // }
    // Serial.print(i16Rps);
    //   Serial.println();
    if(i16Rps != lastRps)
    {
      lastRps = i16Rps;
      // Led.toggle();
      // currTime = micros();
      Serial.print(i16Rps);
      Serial.println();
      // Serial.print(currTime);
      // Serial.println();
      // Serial.print(lastTime);
      // Serial.println();

      // while (1)
      // {
      //   /* code */
      // }
      
    }
  }
  return 0;
}

// interupt service routine of external int0
ISR(INT0_vect)
{
    // Led.set_hi();
    Encoder.updatePos();
    // Led.set_lo();
}

volatile uint8_t ui8PpsCnt = 0;
volatile uint8_t ui8SpeedCtrlCnt = 0;
ISR (TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
    // event to be exicuted every 2ms here
    ui8PpsCnt++;
    ui8SpeedCtrlCnt++;
    if(ui8PpsCnt >= 10)
    {
        /* code to be executed every 20 ms */
        ui8PpsCnt = 0;
        Encoder.updatePps();
        Hbridge.toggle();

    }

    if(ui8SpeedCtrlCnt >= 150)
    {
      /* code to be exectued every 300 ms */
      ui8SpeedCtrlCnt = 0;
      Led.toggle();
    }
}