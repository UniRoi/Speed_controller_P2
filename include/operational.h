#ifndef OPERATIONAL_H
#define OPERATIONAL_H

#include <state.h>

class operational_state : public State
{
public:
  char *getStateName() const override;

  void on_do() override;

  bool has_finished() override;

  void on_entry() override;

  void on_exit() override;

  void on_event1() override;

  void on_event2() override;

  void on_event3() override;
};

#endif