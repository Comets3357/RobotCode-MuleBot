#include "common/gyro.h"
#include "RobotData.h"

void Gyro::RobotInit()
{
    gyro.Reset();
}

void Gyro::RobotPeriodic(const RobotData &robotData, GyroData &gyroData)
{
    gyroData.rawYaw = gyro.GetYaw();
    gyroData.rawRoll = gyro.GetRoll();
    gyroData.angularRate = gyro.GetRawGyroY();

    frc::SmartDashboard::PutNumber("YAW", gyroData.rawRoll);
}

