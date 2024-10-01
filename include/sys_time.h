#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <stdint.h>
// #include <mutex>

class sys_time
{
private:
    volatile uint32_t u32SysTimeCnt;
public:
    sys_time(/* args */);
    ~sys_time();
    void init(void);
    uint32_t Get_SysTimeMs(void);
    void Inc_SysTimeMs(void);
};


#endif