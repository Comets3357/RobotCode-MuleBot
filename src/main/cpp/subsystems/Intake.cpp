#include "subsystems/Intake.h"
#include "RobotData.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Timer.h>
#include <rev/CANSparkMaxLowLevel.h>

void Intake::RobotInit()
{
    intakePivot.RestoreFactoryDefaults();
    intakePivot.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, -29+1);
    intakePivot.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, 0-1);
    intakePivot.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, true);
    intakePivot.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, true);
    intakePivotPIDController.SetOutputRange(-0.3, 0.3);
    intakePivot.SetInverted(false);
    intakePivot.SetSmartCurrentLimit(65);
    intakePivot.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    intakePivot.EnableVoltageCompensation(10);
    intakePivotPIDController.SetP(0.55);
    intakePivot.BurnFlash();
    intakePivotEncoder.SetPosition(0);
    intakeRoller.RestoreFactoryDefaults();
    intakeRoller.SetSmartCurrentLimit(45);
    intakeRoller.EnableVoltageCompensation(10);
    intakeRollerPIDController.SetP(0.0005);
    intakeRollerPIDController.SetD(0.006);
    intakeRollerPIDController.SetFF(0.000221);
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
void Intake::RobotPeriodic(const RobotData &robotData, IntakeData &intakeData)
{
    frc::SmartDashboard::PutNumber("CURRENTVEL", intakeRollerEncoder.GetVelocity());
    frc::SmartDashboard::PutNumber("TARGETVEL", robotData.limelightData.flyWheelVelocity);
    frc::SmartDashboard::PutNumber("AMPS", intakeRollerLeft.GetOutputCurrent());
    frc::SmartDashboard::PutBoolean("LEFT", leftBall);
    frc::SmartDashboard::PutBoolean("RIGHT", rightBall);

    frc::SmartDashboard::PutNumber("PIVOT", intakePivotEncoder.GetPosition());
    frc::SmartDashboard::PutNumber("DIASTNACE", robotData.limelightData.distance);
    if (!isZeroed && (int)timer.Get() > 2 && intakeAbs.GetOutput() != 0)
    {
        intakePivotEncoder.SetPosition(-82.7504*(intakeAbs.GetOutput())+53.1228);
        timer.Stop();
        isZeroed = true;
    }
    if (isZeroed)
    {
        if (robotData.controlData.mode == mode_teleop_manual || robotData.controlData.mode == mode_teleop_sa)
        {
            if (robotData.controlData.mode == mode_teleop_manual)
            { //updates whether or not the robot is in manual or semiAuto mode
                Manual(robotData, intakeData);
            }else{
                //checks is turret is facing forward
                SemiAuto(robotData, intakeData);
            }
            
            
        } 
        else
        {
            //sets powers to 0 if the mode is changed out of climb mode
            intakePivotPIDController.SetReference(0, rev::ControlType::kPosition);
            intakeRoller.Set(0);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);

        }
    }
}

void Intake::Manual(const RobotData &robotData, IntakeData &intakeData)
{
    if (robotData.controllerData.sLBumper > 0.1)
    {
        SetWheelSpeed(2500);
    }
    else if (robotData.controllerData.sRBumper > 0.1)
    {
        SetWheelSpeed(-2500);
    }
    else
    {
        SetWheelSpeed(0);
    }

    if (robotData.controllerData.sLYStick > 0.08 || robotData.controllerData.sLYStick < -0.08)
    {
        intakePivot.Set(robotData.controllerData.sLYStick);
    }
    else
    {
        intakePivot.Set(0);
    }
}

void Intake::SemiAuto(const RobotData &robotData, IntakeData &intakeData)
{
        
        if (robotData.controlData.shooting)
        {
            intakePivotPIDController.SetReference(robotData.limelightData.intakePivotPosition+0.2, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(robotData.limelightData.flyWheelVelocity);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.5);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.5);
            
        }
        else if (robotData.controlData.backShot)
        {
            intakePivotPIDController.SetReference(0, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(-3700);
            
        }
        else if (robotData.controlData.backShotAgainst)
        {
            intakePivotPIDController.SetReference(-0.8, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(-3700);
            
        }
        else if (robotData.controlData.lowShot)
        {
            intakePivotPIDController.SetReference(-4.5, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(-1700);
            
        }
        else if (robotData.controlData.intake)
        {
            intakePivotPIDController.SetReference(-29.5, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(2500);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -1);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 1);
        }
        else if (robotData.controlData.leftEject)
        {
            SetWheelSpeed(-1000);
            if (intakeRollerEncoder.GetVelocity() < -900)
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 1);
                intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.5);
            }
            else
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.5);
                intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.5);
            }
        }
        else if (robotData.controlData.rightEject)
        {
            SetWheelSpeed(-1000);
            if (intakeRollerEncoder.GetVelocity() < -900)
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.5);
                intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -1);
            }
            else
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.5);
                intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.5);
            }
        }
        else
        {
            intakePivotPIDController.SetReference(0, rev::CANSparkMax::ControlType::kPosition);
            SetWheelSpeed(0);
            intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -0.5);
            intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0.5);
        }


        if (robotData.controlData.left)
        {

            if (abs(robotData.limelightData.angleOffHorizontal) < 5)
            {
                intakeRollerLeft.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 1);  
            }
        }

        if (robotData.controlData.right)
        {
            if (abs(robotData.limelightData.angleOffHorizontal) < 5)
            {
                intakeRollerRight.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -1);
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