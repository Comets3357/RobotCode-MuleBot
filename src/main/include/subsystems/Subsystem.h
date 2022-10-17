#pragma once

#include "Constants.h"



struct RobotData;

struct SubsystemData 
{
    bool shooterReady;
};

class Subsystem {

public:
    void RobotInit();
    void RobotPeriodic(const RobotData &robotData, SubsystemData &subsystemData);  
    void DisabledPeriodic(const RobotData &robotData, SubsystemData &subsystemData); 
    void DisabledInit();
private:
    void UpdateData(const RobotData &robotData, SubsystemData &subsystemData);
    void SemiAuto(const RobotData &robotData, SubsystemData &subsystemData);
    void Manual(const RobotData &robotData, SubsystemData &subsystemData);

};