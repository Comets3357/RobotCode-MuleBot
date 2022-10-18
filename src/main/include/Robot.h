//Tells the compiler to not include the header files multiple times
#pragma once

// //Header files from FRC
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/TimedRobot.h>
#include "RobotData.h"
#include "vision/Limelight.h"

//Robot class inherits from TimedRobot
class Robot : public frc::TimedRobot
{

public:
    void RobotInit() override;
    void RobotPeriodic() override;
    void AutonomousInit() override;
    void AutonomousPeriodic() override;
    void TeleopInit() override;
    void TeleopPeriodic() override;
    void DisabledInit() override;
    void DisabledPeriodic() override;
    void TestInit() override;
    void TestPeriodic() override;

private:
    RobotData robotData{};

    // other
    Controller controller{};
    

    // subsystems
    Subsystem subsystem{};
    Drivebase drivebase{};
    Intake intake{};
    Climb climb{};

    // vision
    Limelight limelight{};
};