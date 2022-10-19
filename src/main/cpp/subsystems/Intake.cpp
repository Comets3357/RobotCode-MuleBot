#include "subsystems/Intake.h"
#include "RobotData.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>
#include <rev/CANSparkMaxLowLevel.h>

void Intake::RobotInit()
{
    intakePivot.RestoreFactoryDefaults();
    // intakePivot.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, -74+1);
    // intakePivot.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, 0-1);
    // intakePivot.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, true);
    // intakePivot.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, true);
    intakePivot.SetInverted(false);
    intakePivot.SetSmartCurrentLimit(65);
    intakePivot.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    intakePivot.EnableVoltageCompensation(10);
    intakePivotPIDController.SetP(0.5);
    intakePivot.BurnFlash();
    intakePivotEncoder.SetPosition(0);
    intakeRoller.RestoreFactoryDefaults();
    intakeRoller.SetSmartCurrentLimit(45);
    intakeRoller.EnableVoltageCompensation(10);
    intakeRoller.BurnFlash();
    //rev::CANSparkMaxLowLevel::EnableExternalUSBControl(true);

    frc::SmartDashboard::PutNumber("pivot", 0);
    frc::SmartDashboard::PutNumber("wheel velocity", 0);

    timer.Start();
}

void Intake::SetWheelSpeed(double speed)
{
    if (speed > intakeRollerEncoder.GetVelocity()){
        if (intakeRollerEncoder.GetVelocity() < speed - 350)
        {
            intakeRoller.Set(1);
        }
        else
        {
            intakeRollerPIDController.SetReference(speed, rev::ControlType::kVelocity);
        }
    }
    else if (speed < intakeRollerEncoder.GetVelocity())
    {
        if (intakeRollerEncoder.GetVelocity() > speed + 350)
        {
            intakeRoller.Set(-1);
        }
        else
        {
            intakeRollerPIDController.SetReference(speed, rev::ControlType::kVelocity);
        }
    }
}

void Intake::RobotPeriodic(const RobotData &robotData, IntakeData &intakeData, LimelightData &limelightData)
{
    pivotAngle = frc::SmartDashboard::GetNumber("pivot", 0);
    rollerVelocity = frc::SmartDashboard::GetNumber("wheel velocity", 0);
    frc::SmartDashboard::PutNumber("pivotPos", intakePivotEncoder.GetPosition());
    if (!isZeroed && (int)timer.Get() > 2 && intakeAbs.GetOutput() != 0)
    {
        intakePivotEncoder.SetPosition(127.499-202.573*(intakeAbs.GetOutput()));
        timer.Stop();
        isZeroed = true;
    }
    if (isZeroed)
    {
        if (robotData.controlData.shooting)
        {
            //intakePivotPIDController.SetReference(/*limelightData.intakePivotPosition*/ pivotAngle, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(limelightData.flyWheelVelocity);
            if (intakeRollerEncoder.GetVelocity() > /*limelightData.flyWheelVelocity*/rollerVelocity - 10 && intakeRollerEncoder.GetVelocity() < /*limelightData.flyWheelVelocity*/rollerVelocity + 10)
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.3);
                intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.3);
            }
            else
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
                intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
            }
        }
        else if (robotData.controlData.intake)
        {
            intakePivotPIDController.SetReference(-10, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(-1000);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.3);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.3);
        }
        else
        {
            intakePivotPIDController.SetReference(0, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(0);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
        }
        
        
    intakeData.targetInView = table->GetNumber("tv", 0.0);
    intakeData.xOffset = table->GetNumber("tx", 0.0) * (3.14159265358979323846/180);
    intakeData.yOffset = table->GetNumber("ty", 0.0);
    frc::SmartDashboard::PutNumber("tv", intakeData.targetInView);
    frc::SmartDashboard::PutNumber("vx", intakeData.xOffset);
    frc::SmartDashboard::PutNumber("vy", intakeData.yOffset);

    frc::SmartDashboard::PutNumber("IntakeAbsolute", intakeAbs.GetOutput());
    frc::SmartDashboard::PutNumber("relativeEncoder", intakePivotEncoder.GetPosition());
    intakePivot.Set(robotData.controllerData.sLYStick*0.2);

   
}