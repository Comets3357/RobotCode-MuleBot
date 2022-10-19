#include "common/gyro.h"
#include "RobotData.h"

void Gyro::RobotInit()
{
    gyro.Reset();
}

void Gyro::RobotPeriodic(const RobotData &robotData, GyroData &gyroData)
{
    gyroData.yaw = gyro.GetYaw();
    gyroData.roll = gyro.GetRoll();
    gyroData.angularRate = gyro.GetRawGyroY();
}

