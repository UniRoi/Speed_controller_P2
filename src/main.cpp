#include <encoder.h>
#include <util/delay.h>
#include <digital_out.h>
#include <analog_out.h>
#include <speed_control.h>
#include <Arduino.h>

#include <avr/interrupt.h>
#include <sys_time.h>

#define DEBUG_PRINT

enum eStates
{
  NO_STATE = 0,
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
Digital_in EncFlt(4);

Controller* P_speed = new PI_control(0.01,0.05,0.15, 12500, 1);

sys_time SysTime;

eStates controllerState = eStates::INIT;

void setup()
{
  // put your setup code here, to run once:
  

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

#ifdef DEBUG_PRINT
#include <stdio.h>
#define dbg_print Serial.print
#else
#define dbg_print(...) /**/
#endif

enum eStates fn_checkForTransition(int cmd)
{
  enum eStates Transition = NO_STATE;

  if (cmd == 'o')
  {
    Transition = eStates::OPERATIONAL;
  }
  else if (cmd == 'p')
  {
    Transition = eStates::PRE_OPERATIONAL;
  }
  else if (cmd == 'r')
  {
    Transition = eStates::INIT;
  }

  return Transition;
}

void fn_PrintDbgMsg(const char *msg, uint32_t TimeNow)
{
  static uint32_t LastTime = 0;

  if ((TimeNow - LastTime) > 500)
  {
    dbg_print(msg);
    LastTime = TimeNow;
  }
}

void loop()
{
  uint32_t u32TimeNow;
  eStates eStateTransition;
  bool bFltState = false;
  uint32_t lastTime = 0;
  uint32_t currTime = 0;
  uint16_t ui16EncCnt = 0;
  int16_t i16Rps = 0;

  int i = 0;
  uint8_t brightness{0};
  int new_duty = 0;
  double speed_new = 0;

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

  bFltState = EncFlt.is_hi();
  eStateTransition = fn_checkForTransition(command);

  switch (controllerState)
  {
  case eStates::INIT:
    /* code */
    dbg_print("Init\n");
    command = 0;

    led.set_lo();
    controllerState = eStates::PRE_OPERATIONAL;
    Serial.print("Boot-up\n");
    break;

  case eStates::PRE_OPERATIONAL:
    fn_PrintDbgMsg("pre_op\n", u32TimeNow);
    /* led blinks with 1 Hz */
    if ((u32TimeNow - u32LastTime) > 500)
    {
      led.toggle();
      u32LastTime = u32TimeNow;
    }

    // eStateTransition = fn_checkForTransition(command);
    if (bFltState == true)
    {
      controllerState = eStates::STOPPED;
    }
    else if ((eStateTransition == eStates::INIT) ||
             (eStateTransition == eStates::OPERATIONAL))
    {
      controllerState = eStateTransition;
      command = 0;
    }

    break;

  case eStates::OPERATIONAL:

    fn_PrintDbgMsg("op\n", u32TimeNow);
    /* led is on */
    led.set_hi();

    if (bUpdateSpeed == true)
    {
      i16Rps = Encoder.GetRpm();

      Serial.print(i16Rps);
      Serial.print(" ");

      speed_new = P_speed->update(targetRpm, static_cast<double>(i16Rps));

      new_duty = (constrain(speed_new/targetRpm, 0.1, 0.9)*100);
      
      Serial.print(new_duty);
      Serial.println();

      ana_out.set(new_duty);
      bUpdateSpeed = false;
    }

    

    if (bFltState == true)
    {
      controllerState = eStates::STOPPED;
    }
    else if ((eStateTransition == eStates::INIT) ||
             (eStateTransition == eStates::PRE_OPERATIONAL))
    {
      controllerState = eStateTransition;
      command = 0;
    }

    break;

  default:
  case eStates::STOPPED:
    fn_PrintDbgMsg("stopped\n", u32TimeNow);
    /* default / stopped code */
    /* led blinks with 2 Hz (250 ms for 2 Hz) */
    if ((u32TimeNow - u32LastTime) > 250)
    {
      led.toggle();
      u32LastTime = u32TimeNow;
    }

    if ((eStateTransition == eStates::INIT) ||
        (eStateTransition == eStates::PRE_OPERATIONAL) ||
        (eStateTransition == eStates::OPERATIONAL))
    {
      controllerState = eStateTransition;
      command = 0;
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
