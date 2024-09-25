#include "pre_op.h"
#include "operational.h"
#include "init.h"

#include <Arduino.h>

char *pre_op_state::getStateName() const
{
    return "PreOpState";
}

void pre_op_state::on_do()
{
    Serial.print("PreOp\n");
}

bool pre_op_state::has_finished()
{
    return true;
}

void pre_op_state::on_entry()
{

}

void pre_op_state::on_exit()
{

}

void pre_op_state::on_event1()
{
    /* transition to reset */
    this->context_->transition_to(new init_state);
}

void pre_op_state::on_event2()
{
    /* transition to preop */
}

void pre_op_state::on_event3()
{
    /* transition to op */
    this->context_->transition_to(new operational_state);
}