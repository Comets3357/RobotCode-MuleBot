#include "Robot.h"
#include <frc/livewindow/LiveWindow.h>

void Robot::RobotInit()
{
    subsystem.RobotInit();
    drivebase.RobotInit();
    intake.RobotInit();
    climb.RobotInit();
    limelight.RobotInit();
    gyro.RobotInit();
    //auton.RobotInit(robotData.autonData);
}

void Robot::RobotPeriodic()
{
    subsystem.RobotPeriodic(robotData, robotData.subsystemData);
    drivebase.RobotPeriodic(robotData, robotData.drivebaseData, robotData.autonData);
    intake.RobotPeriodic(robotData, robotData.intakeData);
    climb.RobotPeriodic(robotData, robotData.climbData);
    limelight.RobotPeriodic(robotData, robotData.limelightData);
    gyro.RobotPeriodic(robotData, robotData.gyroData);
    timer.RobotPeriodic(robotData, robotData.timerData);
}

void Robot::AutonomousInit()
{
    drivebase.AutonomousInit(robotData, robotData.drivebaseData, robotData.autonData);
    timer.AutonomousInit(robotData, robotData.timerData);
    //auton.AutonomousInit(robotData.autonData);
}

void Robot::AutonomousPeriodic()
{
    //auton.AutonomousPeriodic(robotData, robotData.autonData, robotData.controlData, robotData.controllerData);
}

void Robot::TeleopInit()
{
    
    timer.TeliopInit(robotData, robotData.timerData);
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
    //auton.DisabledInit();
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