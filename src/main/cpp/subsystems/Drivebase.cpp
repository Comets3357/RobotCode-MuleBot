#include "subsystems/Drivebase.h"
#include "RobotData.h"
#include "frc/smartdashboard/SmartDashboard.h"

void Drivebase::RobotInit()
{
    dbL.RestoreFactoryDefaults();
    dbLF.RestoreFactoryDefaults();
    dbR.RestoreFactoryDefaults();
    dbRF.RestoreFactoryDefaults();
    
    dbRF.Follow(dbR);
    dbLF.Follow(dbL);

    dbL.SetInverted(true);
    dbLF.SetInverted(true);
    dbR.SetInverted(false);
    dbRF.SetInverted(false);

    dbL.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    dbR.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    dbLF.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    dbRF.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);

    dbRPIDController.SetP(0.25);
    dbLPIDController.SetP(0.25);
    dbLPIDController.SetOutputRange(-0.3, 0.3);
    dbRPIDController.SetOutputRange(-0.3, 0.3);

    dbL.BurnFlash();
    dbLF.BurnFlash();
    dbR.BurnFlash();
    dbRF.BurnFlash();
    gyro.Calibrate();

    dbREncoder.SetPosition(0);
    dbLEncoder.SetPosition(0);
}

void Drivebase::RobotPeriodic(const RobotData &robotData, DrivebaseData &drivebaseData, IntakeData &intakeData)
{
    
    frc::SmartDashboard::PutNumber("Yaw", gyro.GetAngle());
    frc::SmartDashboard::PutNumber("encoder", dbREncoder.GetPosition());
    // // if (intakeData.targetInView){
    if (intakeData.shooting)
    {
        //drivebase skew for limelight

        
    }


    if (robotData.controllerData.pLYStick  < -0.08 || robotData.controllerData.pLYStick  > 0.08)
    {
        dbL.Set(robotData.controllerData.pLYStick);
    }
    else if (!intakeData.shooting)
    {
        dbL.Set(0);
    }

    if (robotData.controllerData.pRYStick  < -0.08 || robotData.controllerData.pRYStick  > 0.08)
    {
        dbR.Set(robotData.controllerData.pRYStick);
    }
    else if (!intakeData.shooting)
    {
        dbR.Set(0);
    }
    frc::SmartDashboard::PutNumber("x", robotData.controllerData.pLYStick);
            //dbL.Set(robotData.controllerData.pLYStick);

            //dbR.Set(robotData.controllerData.pRYStick);

}

void Drivebase::DisabledPeriodic(const RobotData &robotData, DrivebaseData &drivebaseData)
{

}

void Drivebase::DisabledInit()
{
    dbL.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    dbR.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    dbLF.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
    dbRF.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
}

void Drivebase::AutonPeriodic(const RobotData &robotData)
{
    //just some auton thing i tried out
    frc::SmartDashboard::PutNumber("error", error);
    frc::SmartDashboard::PutNumber("right encoder", dbREncoder.GetPosition());
    frc::SmartDashboard::PutNumber("left encoder", dbLEncoder.GetPosition());
    switch (autonStage)
    {
    case(0):
    dbREncoder.SetPosition(0);
    dbLEncoder.SetPosition(0);
    autonStage++;
    break;
    case(1):
        dbRPIDController.SetReference(2.93 * ticksPerFoot, rev::CANSparkMax::ControlType::kPosition);
        dbLPIDController.SetReference(-2.93 * ticksPerFoot, rev::CANSparkMax::ControlType::kPosition);
        if (abs(2.93 * (ticksPerFoot) - dbREncoder.GetPosition()) < 1)
        {
            autonStage++;
            dbREncoder.SetPosition(0);
            dbLEncoder.SetPosition(0);
        }
        break;
    case(2):
        dbRPIDController.SetReference(5 * ticksPerFoot, rev::CANSparkMax::ControlType::kPosition);
        dbLPIDController.SetReference(5 * ticksPerFoot, rev::CANSparkMax::ControlType::kPosition);
        if (abs(5 * (ticksPerFoot) - dbREncoder.GetPosition()) < 1)
        {
            autonStage++;
        }
        break;
        case(3):
        dbR.Set(0);
        dbL.Set(0);
        break;
    
    default:
        break;
    }
}
