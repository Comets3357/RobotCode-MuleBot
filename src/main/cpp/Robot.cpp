#include "Robot.h"
#include <frc/livewindow/LiveWindow.h>

void Robot::RobotInit()
{
    subsystem.RobotInit();
    drivebase.RobotInit();
    intake.RobotInit();
    climb.RobotInit();
}

void Robot::RobotPeriodic()
{
    subsystem.RobotPeriodic(robotData, robotData.subsystemData);
    drivebase.RobotPeriodic(robotData, robotData.drivebaseData, robotData.intakeData);
    intake.RobotPeriodic(robotData, robotData.intakeData);
    climb.RobotPeriodic(robotData, robotData.climbData);
}

void Robot::AutonomousInit()
{
    
}

void Robot::AutonomousPeriodic()
{
    //drivebase.AutonPeriodic(robotData);
}

void Robot::TeleopInit()
{
    
}

void Robot::TeleopPeriodic()
{
    controller.TeleopPeriodic(robotData, robotData.controllerData, robotData.controlData);
}

void Robot::DisabledInit()
{
    subsystem.DisabledInit();
    drivebase.DisabledInit();
    climb.DisabledInit();
}

void Robot::DisabledPeriodic() 
{
    subsystem.DisabledPeriodic(robotData, robotData.subsystemData);
    climb.DisabledPeriodic(robotData, robotData.climbData);
}


void Robot::TestInit(){
    frc::LiveWindow::SetEnabled(false); // to block their weird dashboard thing

}

//BENCH TEST CODE
void Robot::TestPeriodic(){
    
}

#ifndef RUNNING_FRC_TESTS
int main()
{
    return frc::StartRobot<Robot>();
}
#endif