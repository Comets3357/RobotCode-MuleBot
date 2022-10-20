#pragma once

#include "Constants.h"
#include <frc/SPI.h>
#include "AHRS.h"


struct RobotData;

struct GyroData 
{
    double rawYaw;
    double rawRoll;
    double angularRate;
};

class Gyro {

public:
    void RobotInit();
    void RobotPeriodic(const RobotData &robotData, GyroData &gyroData);  

private:
    AHRS gyro{frc::SPI::Port::kMXP};


};