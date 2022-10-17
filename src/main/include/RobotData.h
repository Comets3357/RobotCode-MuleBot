#pragma once

// includes other files' data
#include "controller/Controller.h"

#include "subsystems/Subsystem.h"

#include "subsystems/Drivebase.h"

#include "subsystems/Intake.h"

#include "subsystems/Climb.h"

//could be separated into all separate files for the data *from* each subsystem
//commented out variables are not in use
struct RobotData
{
    ControllerData controllerData;
    ControlData controlData;
    DrivebaseData drivebaseData;
    SubsystemData subsystemData;
    IntakeData intakeData;
    ClimbData climbData;
    
};