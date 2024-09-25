#include "operational.h"
#include "pre_op.h"
#include "init.h"

#include <Arduino.h>

char *operational_state::getStateName() const
{
    return "OperationalState";
}

void operational_state::on_do()
{
    Serial.print("Init\n");
}

bool operational_state::has_finished()
{
    return true;
}

void operational_state::on_entry()
{

}

void operational_state::on_exit()
{

}

void operational_state::on_event1()
{
    /* transition to reset */
    this->context_->transition_to(new init_state);
}

void operational_state::on_event2()
{
    /* transition to pre op */
    this->context_->transition_to(new pre_op_state);
}

void operational_state::on_event3()
{
    /* transition to op */
}