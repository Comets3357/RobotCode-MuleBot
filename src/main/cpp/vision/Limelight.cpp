#include "vision/Limelight.h"
#include "RobotData.h"

void Limelight::RobotInit()
{
    mTable->PutNumber("ledMode", 0);

    /*
    *  velocity map
    */

    mVelocityMap.push_back(1000); // 0 feet
    mVelocityMap.push_back(1200); // 2 feet

    /*
    *  pivot position map
    */

    mPivotPositionMap.push_back(0.5); // 0 feet
    mPivotPositionMap.push_back(0.5); // 2 feet

}

void Limelight::RobotPeriodic(const RobotData &robotData, LimelightData &limelightData)
{
    limelightData.angleOffHorizontal = mTable->GetNumber("tx", 0.0);

    mYOffset = mTable->GetNumber("ty", 0.0);

    if (robotData.intakeData.shooting) 
    {
        mTable->PutNumber("pipeline", 0);

        limelightData.distance = GetDistanceFromHub();
        limelightData.flyWheelVelocity = GetFlyWheelVelocity(limelightData);
        limelightData.intakePivotPosition = GetIntakePivotPosition(limelightData);
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
    double distanceEveryTwoFeet = limelightData.distance / 24; // 24 because divide by 12 and then divide by every two fee
    int lower = floor(distanceEveryTwoFeet); 
    int upper = lower + 1;
    double lowerVelocity = 0;
    double upperVelocity = 0;

    if (upper > mVelocityMap.size())
    {
        upper = mVelocityMap.back();
    }

    if (lower > mVelocityMap.size())
    {
        lower = mVelocityMap.back();
    }

    lowerVelocity = mVelocityMap.at(lower);
    upperVelocity = mVelocityMap.at(upper);

    return (((upperVelocity - lowerVelocity)/24) * ((distanceEveryTwoFeet - lower) * 24) + lowerVelocity);
}

double Limelight::GetIntakePivotPosition(LimelightData &limelightData)
{
    double distanceEveryTwoFeet = limelightData.distance / 24; // 24 because divide by 12 and then divide by every two fee
    int lower = floor(distanceEveryTwoFeet); 
    int upper = lower + 1;
    double lowerVelocity = 0;
    double upperVelocity = 0;

    if (upper > mPivotPositionMap.size())
    {
        upper = mPivotPositionMap.size();
    }

    if (lower > mPivotPositionMap.size())
    {
        lower = mPivotPositionMap.size();
    }

    lowerVelocity = mPivotPositionMap.at(lower);
    upperVelocity = mPivotPositionMap.at(upper);

    return (((upperVelocity - lowerVelocity)/24) * ((distanceEveryTwoFeet - lower) * 24) + lowerVelocity);
}