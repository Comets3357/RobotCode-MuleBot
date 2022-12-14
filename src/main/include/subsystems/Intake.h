#pragma once

#include "Constants.h"
#include <rev/CANSparkMax.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableValue.h>

#include <rev/CANDigitalInput.h>
#include <frc/DigitalInput.h>
#include <frc/DutyCycle.h>
#include <frc/Timer.h>
#include <ctre/Phoenix.h>

struct RobotData;

struct IntakeData
{
    // double shooting = 0;
    double xOffset;
    bool toRotation = false;
    bool targetInView = false;
    double yOffset = 0;
    int ballCount = 1;
    bool shooting = false;

};

enum SetPoint
{
    NONE,
    POINT1,
    POINT2,
    POINT3,
    POINT4
};

class Intake
{
public:
    void RobotInit();
    void RobotPeriodic(const RobotData &robotData, IntakeData &intakeData);  
    void DisabledPeriodic(const RobotData &robotData, IntakeData &intakeData); 
    void DisabledInit();

    void SetWheelSpeed(double speed);

    void Manual(const RobotData &robotData, IntakeData &intakeData);
    void SemiAuto(const RobotData &robotData, IntakeData &intakeData);

private:

    

    std::shared_ptr<nt::NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight"); 
    SetPoint setPoint = NONE;

    double targetRollerSpeed;
    bool shootMode = false;
    double targetPivotPosition;

    bool ballInLeft = false;
    bool ballInRight = false;
    bool readyShoot = true;
    int ballsShooting = 0;

    bool isZeroed = false;

    frc::Timer timer;

    bool shooting = false;


    //place holder
    double pivotAngle = 0;
    double rollerVelocity = 0;

    bool leftBall = false;
    bool rightBall = false;


    rev::CANSparkMax intakePivot = rev::CANSparkMax(intakePivotDeviceID, rev::CANSparkMaxLowLevel::MotorType::kBrushless);
    rev::SparkMaxRelativeEncoder intakePivotEncoder = intakePivot.GetEncoder();
    rev::SparkMaxPIDController intakePivotPIDController = intakePivot.GetPIDController();
    rev::CANSparkMax intakeRoller = rev::CANSparkMax(intakeRollerID, rev::CANSparkMaxLowLevel::MotorType::kBrushless);
    rev::SparkMaxRelativeEncoder intakeRollerEncoder = intakeRoller.GetEncoder();
    rev::SparkMaxPIDController intakeRollerPIDController = intakeRoller.GetPIDController();
   

    ctre::phoenix::motorcontrol::can::TalonSRX intakeRollerLeft{intakeRollerLeftID};
    ctre::phoenix::motorcontrol::can::TalonSRX intakeRollerRight{intakeRollerRightID};

    frc::DigitalInput m_input{0};
    //THIS IS THE ABSOLUTE ENCODER
    frc::DutyCycle intakeAbs = frc::DutyCycle{m_input};

};