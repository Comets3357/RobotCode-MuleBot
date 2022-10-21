#pragma once

#include "Constants.h"
#include <frc/Timer.h>



struct RobotData;

struct TimerData 
{
    double secSinceEnabled = 0;
};

class Timer {

public:
    void AutonomousInit(const RobotData &robotData, TimerData &timerData);
    void TeliopInit(const RobotData &robotData, TimerData &timerData);
    void RobotPeriodic(const RobotData &robotData, TimerData &timerData);  

private:
    frc::Timer timer;


};