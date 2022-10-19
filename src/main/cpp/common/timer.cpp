#include "common/timer.h"
#include "RobotData.h"

void Timer::AutonomousInit(const RobotData &robotData, TimerData &timerData)
{
    timer.Reset();
    timer.Start();
}

void Timer::TeliopInit(const RobotData &robotData, TimerData &timerData)
{
    timer.Reset();
    timer.Start();
}

void Timer::RobotPeriodic(const RobotData &robotData, TimerData &timerData)
{
    timerData.secSinceEnabled = (double)timer.Get();
}
