#include "subsystems/Intake.h"
#include "RobotData.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>
#include <rev/CANSparkMaxLowLevel.h>

void Intake::RobotInit()
{
    intakePivot.RestoreFactoryDefaults();
    intakePivot.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, -74+1);
    intakePivot.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, 0-1);
    intakePivot.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, true);
    intakePivot.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, true);
    intakePivot.SetInverted(false);
    intakePivot.SetSmartCurrentLimit(65);
    intakePivot.EnableVoltageCompensation(10);
    intakePivotPIDController.SetP(2);
    intakePivot.BurnFlash();
    intakePivotEncoder.SetPosition(0);
    intakeRoller.RestoreFactoryDefaults();
    intakeRoller.SetSmartCurrentLimit(45);
    intakeRoller.EnableVoltageCompensation(10);
    intakeRoller.BurnFlash();
    //rev::CANSparkMaxLowLevel::EnableExternalUSBControl(true);

    frc::SmartDashboard::PutNumber("pivot", -25);
    frc::SmartDashboard::PutNumber("shootvelocity", -0.8);
    timer.Start();
}

void Intake::RobotPeriodic(const RobotData &robotData, IntakeData &intakeData)
{
    Pivot = frc::SmartDashboard::GetNumber("pivot", -25);
    shooterWheelVelocity = frc::SmartDashboard::GetNumber("shooterWheelVelocity", -0.8);
    if (!isZeroed && (int)timer.Get() > 2 && intakeAbs.GetOutput() != 0)
    {
        intakePivotEncoder.SetPosition(127.499-202.573*(intakeAbs.GetOutput()));
        timer.Stop();
        isZeroed = true;
    }
    if (isZeroed)
    {
        //shooter code kinda scuffed rn
        //move to manual and semiauto functions
        if (robotData.controllerData.sXBtnToggled)
        {
            shooting = !shooting;
            intakeData.shooting = !intakeData.shooting;
        }
        if (robotData.controlData.mode == mode_teleop_sa)
        {
            
        }
        if (robotData.controllerData.sRBumper)
        {
            intakePivotPIDController.SetReference(-74, rev::CANSparkMax::ControlType::kPosition);
            intakeRoller.Set(0.3);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.3);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.3);
        }
        else if (shooting)
        {
            intakePivotPIDController.SetReference(-25, rev::CANSparkMax::ControlType::kPosition);
            intakeRoller.Set(-0.8);
            if (robotData.controllerData.sYBtn)
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 1);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -1);
            }
            else
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
            }
        }
        else if (!shooting)
        {
            intakeRoller.Set(0);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
            intakePivotPIDController.SetReference(0, rev::CANSparkMax::ControlType::kPosition);
        }
    }
    intakeData.targetInView = table->GetNumber("tv", 0.0);
    intakeData.xOffset = table->GetNumber("tx", 0.0) * (3.14159265358979323846/180);
    intakeData.yOffset = table->GetNumber("ty", 0.0);
    frc::SmartDashboard::PutNumber("tv", intakeData.targetInView);
    frc::SmartDashboard::PutNumber("vx", intakeData.xOffset);
    frc::SmartDashboard::PutNumber("vy", intakeData.yOffset);

    frc::SmartDashboard::PutNumber("IntakeAbsolute", intakeAbs.GetOutput());
    frc::SmartDashboard::PutNumber("relativeEncoder", intakePivotEncoder.GetPosition());

   
}