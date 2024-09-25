#include "init.h"
#include "pre_op.h"

#include <Arduino.h>

char *init_state::getStateName() const
{
    return "InitState";
}

void init_state::on_do()
{
    Serial.print("Init\n");
}

bool init_state::has_finished()
{
    return true;
}

void init_state::on_entry()
{
    // do initialization here

    // after finished init go to pre op
    on_event2();
}

void init_state::on_exit()
{

}

void init_state::on_event1()
{
    /* transition to reset */
}

void init_state::on_event2()
{
    /* transition to pre op */
    this->context_->transition_to(new pre_op_state);
}

void init_state::on_event3()
{
    /* transition to op */
}