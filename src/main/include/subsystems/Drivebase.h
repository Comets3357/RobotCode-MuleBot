#pragma once

#include "Constants.h"
#include "subsystems/Intake.h"

#include <rev/CANSparkMax.h>
#include <math.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/SPI.h>
#include "AHRS.h"



struct RobotData;

struct DrivebaseData 
{

};

class Drivebase {

public:
    void RobotInit();
    void RobotPeriodic(const RobotData &robotData, DrivebaseData &drivebaseData, IntakeData &intakeData);  
    void DisabledPeriodic(const RobotData &robotData, DrivebaseData &drivebaseData); 

    void AutonPeriodic(const RobotData &robotData);

    void DisabledInit();
private:

    double ticksPerFoot = 4.492;
    int autonStage = 0;
    double error = 0;
    double motorPower = 0;

    AHRS gyro{frc::SPI::Port::kMXP};

    rev::CANSparkMax dbL = rev::CANSparkMax(leftLeadDeviceID, rev::CANSparkMaxLowLevel::MotorType::kBrushless);
    rev::CANSparkMax dbLF = rev::CANSparkMax(leftFollowDeviceID, rev::CANSparkMaxLowLevel::MotorType::kBrushless);

    rev::CANSparkMax dbR = rev::CANSparkMax(rightFollowDeviceID, rev::CANSparkMaxLowLevel::MotorType::kBrushless);
    rev::CANSparkMax dbRF = rev::CANSparkMax(rightLeadDeviceID, rev::CANSparkMaxLowLevel::MotorType::kBrushless);

    rev::SparkMaxRelativeEncoder dbREncoder = dbR.GetEncoder();
    rev::SparkMaxRelativeEncoder dbLEncoder = dbL.GetEncoder();

    rev::SparkMaxPIDController dbRPIDController = dbR.GetPIDController();
    rev::SparkMaxPIDController dbLPIDController = dbL.GetPIDController();

};