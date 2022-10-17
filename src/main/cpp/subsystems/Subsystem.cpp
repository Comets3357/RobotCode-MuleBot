#include "subsystems/Subsystem.h"
#include "RobotData.h"

void Subsystem::RobotInit()
{
    // Robot Init
}

void Subsystem::RobotPeriodic(const RobotData &robotData, SubsystemData &subsystemData)
{
    UpdateData(robotData, subsystemData);

    if (robotData.controlData.mode == 1)
    {
        Manual(robotData, subsystemData);
    }
    else
    {
        SemiAuto(robotData, subsystemData);
    }
}

void Subsystem::Manual(const RobotData &robotData, SubsystemData &subsystemData)
{
    // Manual
}

void Subsystem::SemiAuto(const RobotData &robotData, SubsystemData &subsystemData)
{
    // SemiAuto
}

void Subsystem::DisabledPeriodic(const RobotData &robotData, SubsystemData &subsystemData)
{
    // Disabled Periodic
}

void Subsystem::DisabledInit()
{
    // Disabled Init
}

void Subsystem::UpdateData(const RobotData &robotData, SubsystemData &subsystemData)
{
    // Update Data
}
