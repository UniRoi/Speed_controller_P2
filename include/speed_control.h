#ifndef P_CONTROL_H
#define P_CONTROL_H

#include <encoder.h>

class P_control
{
private:
    float fKp_;
public:
    P_control(float fKp);
    ~P_control();
    double update(double ref, double actual);
};






#endif