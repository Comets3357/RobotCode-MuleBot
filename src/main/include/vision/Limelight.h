#pragma once

#include "Constants.h"

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableValue.h>
#include <wpi/span.h>
#include <cmath>
#include <deque>
#include <frc/DriverStation.h>
#include <frc/smartdashboard/SmartDashboard.h>

struct RobotData;

struct LimelightData 
{
    double distance = 0;
    double angleOffHorizontal = 0;
    double flyWheelVelocity = 0;
    double intakePivotPosition;
};

class Limelight 
{
public:

void RobotInit();
void RobotPeriodic(const RobotData &robotData, LimelightData &limelightData);

private:

double GetDistanceFromHub();
double GetFlyWheelVelocity(LimelightData &limelightData);
double GetIntakePivotPosition(LimelightData &limelightData);

std::shared_ptr<nt::NetworkTable> mTable = nt::NetworkTableInstance::GetDefault().GetTable("limelight");

double mHubHeight = 104;
double mCameraHeight = 30.4;
double mCameraAngle = 43;

double mXOffset = 0;
double mYOffset = 0;


};


