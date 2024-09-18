#include <encoder.h>
#include <util/delay.h>
// #include <digital_out.h>
#include <analog_out.h>
#include <speed_control.h>
#include <Arduino.h>

#include <avr/interrupt.h>

static uint16_t lastEncCnt = 0;
static int16_t lastRps = 0;
encoder Encoder(2, 3);
Analog_out ana_out(1);

P_control P_speed(2.1);

uint16_t targetRpm = 5000;

volatile bool bUpdateSpeed;

int main(void)
{
  uint32_t lastTime = 0;
  uint32_t currTime = 0;
  uint16_t ui16EncCnt = 0;
  int16_t i16Rps = 0;

  bUpdateSpeed = false;

  ana_out.init(10);

  // here interrupt registers are set
  Encoder.init();
  sei();

  // Add serial for part 2
  Serial.begin(115200);

  int i = 0;
  uint8_t brightness{0};
  int new_duty = 0;
  double speed_new = 0;
  while (1)
  {
    // _delay_ms(10);

    if (bUpdateSpeed == true)
    {
      i16Rps = Encoder.GetRpm();

      Serial.print(i16Rps);
      Serial.print(" ");

      speed_new = P_speed.update(targetRpm, static_cast<double>(i16Rps));

      // Serial.print(speed_new);
      // Serial.print(" ");

      new_duty = (constrain(speed_new/targetRpm, 0.1, 0.9)*100);
      
      // Serial.print(speed_new);
      // Serial.print(" ");
      Serial.print(new_duty);
      Serial.println();

      ana_out.set(new_duty);
      // Serial.println();
      bUpdateSpeed = false;
    }

    // if (i16Rps != lastRps)
    // {
    //   lastRps = i16Rps;

    //   Serial.print(i16Rps);
    //   Serial.println();
    //   // Serial.print(currTime);
    //   // Serial.println();
    //   // Serial.print(lastTime);
    //   // Serial.println();

    //   // while (1)
    //   // {
    //   //   /* code */
    //   // }
    // }
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
ISR(TIMER0_COMPA_vect) // timer0 overflow interrupt
{
  // event to be exicuted every 2ms here
  ui8PpsCnt++;
  ui8SpeedCtrlCnt++;
  if (ui8PpsCnt >= 10)
  {
    /* code to be executed every 20 ms */
    ui8PpsCnt = 0;
    Encoder.updatePps();
  }

  if (ui8SpeedCtrlCnt >= 150)
  {
    /* code to be exectued every 300 ms */
    ui8SpeedCtrlCnt = 0;

    bUpdateSpeed = true;
  }
}

ISR(TIMER1_COMPA_vect)
{
  // action to be taken at the start of the on-cycle
  ana_out.pin_out.set_hi();
}

ISR(TIMER1_COMPB_vect)
{
  // action to be taken at the start of the off-cycle
  ana_out.pin_out.set_lo();
}
