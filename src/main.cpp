#include <encoder.h>
#include <util/delay.h>
#include <digital_out.h>
#include <analog_out.h>
#include <speed_control.h>
#include <Arduino.h>

#include <avr/interrupt.h>
#include <sys_time.h>

// Application#
// #include "init.h"
// #include "pre_op.h"
// #include "operational.h"
// #include "stopped.h"

enum eStates
{
  INIT = 1,
  PRE_OPERATIONAL = 3,
  OPERATIONAL = 6,
  STOPPED = 10
};

static uint32_t u32LastTime;
static uint16_t lastEncCnt = 0;
static int16_t lastRps = 0;
volatile bool bUpdateSpeed;
static int command = 0;
uint16_t targetRpm = 5000;

encoder Encoder(2, 3);
Analog_out ana_out(1);
Digital_out led(5);

P_control P_speed(2.1);

sys_time SysTime;
// Context *context;

eStates controllerState = eStates::INIT;

void setup()
{
  // put your setup code here, to run once:
  uint32_t lastTime = 0;
  uint32_t currTime = 0;
  uint16_t ui16EncCnt = 0;
  int16_t i16Rps = 0;

  bUpdateSpeed = false;

  ana_out.init(10);
  led.init();

  // here interrupt registers are set
  Encoder.init();
  sei();

  // Add serial for part 2
  Serial.begin(115200);

  int i = 0;
  uint8_t brightness{0};
  int new_duty = 0;
  double speed_new = 0;

  SysTime.init();
  u32LastTime = SysTime.Get_SysTimeMs();
  // context = new Context(new init_state);
}

void loop()
{
  uint32_t u32TimeNow;

  u32TimeNow = SysTime.Get_SysTimeMs();
  
  // send data only when you receive data:
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    command = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(command, DEC);
  }

  switch (controllerState)
  {
  case eStates::INIT:
    /* code */
    led.set_lo();
    controllerState = eStates::PRE_OPERATIONAL;
    break;

  case eStates::PRE_OPERATIONAL:
    /* led blinks with 1 Hz */
    if((u32TimeNow - u32LastTime) > 500)
    {
      led.toggle();
      u32LastTime = u32TimeNow;
    }

    break;

  case eStates::OPERATIONAL:
    /* led is on */
    led.set_hi();

    break;

  default:
  case eStates::STOPPED:
    /* default / stopped code */
    /* led blinks with 2 Hz (250 ms for 2 Hz) */
    if((u32TimeNow - u32LastTime) > 250)
    {
      led.toggle();
      u32LastTime = u32TimeNow;
    }

    break;
  }

}

// interupt service routine of external int0
ISR(INT0_vect)
{
  Encoder.updatePos();
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


ISR(TIMER2_COMPA_vect) // timer2 overflow interrupt
{
    SysTime.Inc_SysTimeMs();
}
