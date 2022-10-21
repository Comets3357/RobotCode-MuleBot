#include "controller/Controller.h"
#include "RobotData.h"

// for updating states of control variables (to be accessed by other subsystems)
void Controller::updateControlData(const RobotData &robotData, const ControllerData &controllerData, ControlData &controlData)
{
    controlData.shift = controllerData.sLBumper;
    
    switch (controllerData.sDPad) {
        case -1:
            break;
        case 0: // up
            controlData.mode = mode_teleop_manual;
            break;
        case 90:    // right
            controlData.mode = mode_teleop_sa;
            break;
        case 180:   // down
            controlData.mode = mode_climb_manual;
            break;
        case 270:   // left
            controlData.mode = mode_climb_sa;
            break;
        default:
            controlData.mode = mode_teleop_sa;
            break;
    }



    //driveBase
    controlData.turnResponsive = controllerData.pRShoulderSwitch;
    if (controlData.turnResponsive)
    {
        controlData.maxStraight = 1;
        controlData.maxTurn = 1;
    }
    else
    {
        controlData.maxStraight = 1;
        controlData.maxTurn = 0.3;
    }

    controlData.dbInverted = controllerData.pLShoulderSwitch;
    // if you're inverted then you swtich sides for driving so it's intuitive
    if (controlData.dbInverted)
    {
        controlData.lDrive = -controllerData.pRYStick;
        controlData.rDrive = -controllerData.pLYStick;
    }
    else
    {
        controlData.lDrive = controllerData.pLYStick;
        controlData.rDrive = controllerData.pRYStick;
    }

    if (controlData.mode != mode_teleop_sa) {
        controlData.shootMode = shootMode_none;
        return;
    }

    if (robotData.controlData.shooting) {
        controlData.shootMode == shootMode_vision;
    }
    else
    {
        controlData.shootMode = shootMode_none;
    }

    if (robotData.controllerData.sYBtnToggled && !controlData.shift)
    {
        controlData.shooting = !controlData.shooting;
        if (controlData.shooting == true)
        {
            controlData.backShot = false;
            controlData.lowShot = false;
            controlData.backShotAgainst = false;
        }
    }
    if (robotData.controllerData.sYBtnToggled && controlData.shift)
    {
        controlData.backShot = !controlData.backShot;
        if (controlData.backShot == true)
        {
            controlData.shooting = false;
            controlData.lowShot = false;
            controlData.backShotAgainst = false;
        }
    }

    if (robotData.controllerData.sBBtnToggled && controlData.shift)
    {
        controlData.lowShot = !controlData.backShot;
        if (controlData.lowShot == true)
        {
            controlData.shooting = false;
            controlData.backShot = false;
            controlData.backShotAgainst = false;
        }
    }

    if (robotData.controllerData.sXBtnToggled && controlData.shift)
    {
        controlData.backShotAgainst = !controlData.backShotAgainst;
        if (controlData.backShotAgainst == true)
        {
            controlData.shooting = false;
            controlData.lowShot = false;
            controlData.backShot = false;
        }
    }
    //controlData.backShot = robotData.controllerData.sYBtn && controlData.shift;
    controlData.leftEject = robotData.controllerData.sLCenterBtn;
    controlData.rightEject = robotData.controllerData.sRCenterBtn;
    controlData.intake = robotData.controllerData.sRTrigger>0.1;

    controlData.left = robotData.controllerData.sXBtn && !controlData.shift;
    controlData.right = robotData.controllerData.sBBtn && ! controlData.shift;
}