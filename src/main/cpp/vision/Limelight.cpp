#include "vision/Limelight.h"
#include "RobotData.h"

void Limelight::RobotInit()
{
    mTable->PutNumber("ledMode", 0);
}

void Limelight::RobotPeriodic(const RobotData &robotData, LimelightData &limelightData)
{
    limelightData.angleOffHorizontal = mTable->GetNumber("tx", 0.0);

    mYOffset = mTable->GetNumber("ty", 0.0);

    if (robotData.intakeData.shooting) 
    {
        mTable->PutNumber("pipeline", 0);

        limelightData.distance = GetDistanceFromHub();



    }
    else 
    {
        mTable->PutNumber("pipeline", 1);
    }

   
}

double Limelight::GetDistanceFromHub()
{
    return (mHubHeight - mCameraHeight) / (tan(mCameraAngle + mYOffset));
}


double Limelight::GetFlyWheelVelocity(LimelightData &limelightData)
{

}

double Limelight::GetIntakePivotPosition(LimelightData &limelightData)
{

}


