#include <speed_control.h>

P_control::P_control(float fKp)
{
    fKp_ = fKp;
}

P_control::~P_control()
{
}

double P_control::update(double ref, double actual)
{
    return (ref - actual) * fKp_;
}