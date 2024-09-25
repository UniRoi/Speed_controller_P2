#include "stopped.h"
#include "pre_op.h"
#include "init.h"
#include "operational.h"

#include <Arduino.h>

char *stopped_state::getStateName() const
{
    return "StoppedState";
}

void stopped_state::on_do()
{
    // led should blink at 2Hz

    // check for states
}

bool stopped_state::has_finished()
{
    return true;
}

void stopped_state::on_entry()
{

}

void stopped_state::on_exit()
{

}

void stopped_state::on_event1()
{
    /* transition to reset */
    this->context_->transition_to(new init_state);
}

void stopped_state::on_event2()
{
    /* transition to pre op */
    this->context_->transition_to(new pre_op_state);
}

void stopped_state::on_event3()
{
    /* transition to op */
    this->context_->transition_to(new operational_state);
}