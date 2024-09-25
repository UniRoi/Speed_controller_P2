// State Behaviour based on the C++ example at Refactoring Guru

/**
 * The base State class declares methods that all concrete states should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */

#ifndef STATE_H
#define STATE_H

class Context; // Forward declaration to allow the definition of a pointer to Context without compile error

class State
{
  /**
   * @var Context
   */

protected:
  Context *context_;

public:
  virtual ~State()
  {
  }

  void set_context(Context *context)
  {
    this->context_ = context;
  }

  virtual char *getStateName() const = 0;

  virtual void on_do() = 0;

  virtual bool has_finished() = 0;

  virtual void on_entry() = 0;

  virtual void on_exit() = 0;

  virtual void on_event1() = 0;

  virtual void on_event2() = 0;

  virtual void on_event3() = 0;
};

/**
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */

class Context
{
  /**
   * @var State A reference to the current state of the Context.
   */

private:
  State *state_;

public:
  Context(State *state) : state_(nullptr)
  {
    this->transition_to(state);
  }

  ~Context()
  {
    delete state_;
  }

  /**
   * The Context allows changing the State object at runtime.
   */

  void transition_to(State *state)
  {
    if (this->state_ != nullptr)
    {
      this->state_->on_exit();
      delete this->state_;
    }

    this->state_ = state;

    this->state_->set_context(this);

    this->state_->on_entry();
  }

  /**
   * The Context delegates part of its behavior to the current State object.
   */

  void do_work()
  {
    this->state_->on_do();
  }

  bool is_done()
  {
    return this->state_->has_finished();
  }

  void event1()
  {
    this->state_->on_event1();
  }

  void event2()
  {
    this->state_->on_event2();
  }

  void event3()
  {
    this->state_->on_event3();
  }

  char *getCurrentState() const
  {
    return state_->getStateName();
  }

  State *getState() const
  {
    return this->state_;
  }

};

#endif