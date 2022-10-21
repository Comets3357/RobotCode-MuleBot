#include "RobotData.h"

void Limelight::RobotInit()
{
    mTable->PutNumber("ledMode", 0);

    /*
    *  velocity map
    */

    mVelocityMap.push_back(-3000); // 0 feet
    mVelocityMap.push_back(-3000); // 2 feet
    mVelocityMap.push_back(-3000); // 4 feet
    mVelocityMap.push_back(-3100); // 6 feet
    mVelocityMap.push_back(-3190); // 8 feet
    mVelocityMap.push_back(-3350); // 10 feet
    mVelocityMap.push_back(-3400); // 12 feet
    mVelocityMap.push_back(-3600); // 14 feet
    mVelocityMap.push_back(-3700); // 16 feet
    mVelocityMap.push_back(-4600); // 16 feet

    /*
    *  pivot position map
    */

    mPivotPositionMap.push_back(-4); // 0 feet
    mPivotPositionMap.push_back(-4); // 2 feet
    mPivotPositionMap.push_back(-4); // 4 feet
    mPivotPositionMap.push_back(-5); // 6 feet
    mPivotPositionMap.push_back(-5.6); // 8 feet
    mPivotPositionMap.push_back(-6); // 10 feet
    mPivotPositionMap.push_back(-6.6); // 12 feet
    mPivotPositionMap.push_back(-6.7); // 14 feet
    mPivotPositionMap.push_back(-7.7); // 16 feet
    mPivotPositionMap.push_back(-7.7); // 18 feet

}

void Limelight::RobotPeriodic(const RobotData &robotData, LimelightData &limelightData)
{
    limelightData.angleOffHorizontal = mTable->GetNumber("tx", 0.0);

    mYOffset = mTable->GetNumber("ty", 0.0);
    limelightData.distance = GetDistanceFromHub();
    limelightData.flyWheelVelocity = GetFlyWheelVelocity(limelightData);
        limelightData.intakePivotPosition = GetIntakePivotPosition(limelightData);

    if (robotData.intakeData.shooting) 
    {
        mTable->PutNumber("pipeline", 0);

        //limelightData.distance = GetDistanceFromHub();
        
    }
    else 
    {
        mTable->PutNumber("pipeline", 0);
    }

   
}

double Limelight::GetDistanceFromHub()
{
    return 5+(mHubHeight - mCameraHeight) / (tan((mCameraAngle + mYOffset) * (3.1415926 / 180.0)));
}


double Limelight::GetFlyWheelVelocity(LimelightData &limelightData)
{
    double distanceEveryTwoFeet = limelightData.distance / 24; // 24 because divide by 12 and then divide by every two fee
    int lower = floor(distanceEveryTwoFeet); 
    int upper = lower + 1;
    double lowerVelocity = 0;
    double upperVelocity = 0;

    if (upper >= mVelocityMap.size())
    {
        upper = 9;
    }

    if (lower >= mVelocityMap.size())
    {
        lower = 9;
    }

    frc::SmartDashboard::PutNumber("BACK", mVelocityMap.back());

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

    if (upper >= mPivotPositionMap.size())
    {
        upper = mPivotPositionMap.size()-1;
    }

    if (lower >= mPivotPositionMap.size())
    {
        lower = mPivotPositionMap.size()-1;
    }

    lowerVelocity = mPivotPositionMap.at(lower);
    upperVelocity = mPivotPositionMap.at(upper);

    return (((upperVelocity - lowerVelocity)/24) * ((distanceEveryTwoFeet - lower) * 24) + lowerVelocity);



//rev distance rpm
    //-4.7, 40, -3000
}