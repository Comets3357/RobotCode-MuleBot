    #include "subsystems/Climb.h"
#include "RobotData.h"

// 1 overtild the bot so that there cannot be any bounceback
// 2 wait until the bot is swinging forward before tilting the bot towards the bar
// 3 when pulling off of the bar only pull when the gyro senses that the arms are on the bar
// 4 shorten the extent when going up to the next bar so it will lock when moving onto the bar
// 5 wait until the bot stops swinging

void Climb::RobotInit()
{
    //do other init stuff (probably more)
    climbArms.RestoreFactoryDefaults();
    climbElevator.RestoreFactoryDefaults();

    //do other init stuff (probably more)
    climbArms.RestoreFactoryDefaults();
    climbElevator.RestoreFactoryDefaults();

    //sets the inversion of the motors
    climbArms.SetInverted(false);
    climbElevator.SetInverted(false);
    climbArms.SetSmartCurrentLimit(45);
    climbElevator.SetSmartCurrentLimit(80);
    //climbElevator.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward,140);
    //climbArms.SetSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward,250);

    //motor idlemode
    climbElevator.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    climbArms.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);

    //resets encoders
    climbElevatorEncoder.SetPosition(0);
    climbArmsEncoder.SetPosition(0);

    //PIDS
    climbElevator_pidController.SetP(0.2, 0); //off bar
    climbElevator_pidController.SetOutputRange(-elevatorSpeed, elevatorSpeed, 0);
    climbElevator_pidController.SetP(1, 1); //on bar temp
    climbElevator_pidController.SetOutputRange(-elevatorSpeed, elevatorSpeed, 1);

    climbArms_pidController.SetP(0.27, 0);
    climbArms_pidController.SetOutputRange(-armsSpeed, armsSpeed, 0);
    climbArms_pidController.SetP(0.5, 1);
    climbArms_pidController.SetOutputRange(-armsSpeed, armsSpeed, 1);

    climbElevator.BurnFlash();
    climbArms.BurnFlash();
    
}

void Climb::RobotPeriodic(const RobotData &robotData, ClimbData &climbData)
{

    frc::SmartDashboard::PutNumber("encoder Value", climbElevatorEncoder.GetPosition());
    frc::SmartDashboard::PutNumber("STAGE", stage);
    angle = gyro.GetRoll();
    angularRate = gyro.GetRawGyroY();
    frc::SmartDashboard::PutNumber("ROLL", angle);
    frc::SmartDashboard::PutNumber("GYROY", angularRate);
    //updates
    updateData(robotData, climbData);

    //checks if the robot is in climb mode
    if (robotData.controlData.mode == mode_climb_sa || robotData.controlData.mode == mode_climb_manual)
    {
        if (robotData.controlData.mode == mode_climb_manual)
        { //updates whether or not the robot is in manual or semiAuto mode
            manual(robotData, climbData);
        }else{
            //checks is turret is facing forward
            semiAuto(robotData, climbData);
        }
        
        
    } 
    else
    {
        //sets powers to 0 if the mode is changed out of climb mode
        climbElevator.Set(0);
        climbArms.Set(0);
    }


    // if (climbArmsAbs.GetOutput() > 0.811 && climbArms.Get() > 0)
    // {
    //     climbArms.Set(0);
    // }
}

//manual
void Climb::manual(const RobotData &robotData, ClimbData &climbData)
{
    
    
    
    

   
    //dead zone
        if (robotData.controllerData.sRYStick < -0.08 || robotData.controllerData.sRYStick > 0.08)
        {
            //sets arm power to joystick when joystick is outside of deadzone
            climbArms.Set(-robotData.controllerData.sRYStick); //sets the power to 0 so the arms stop moving
        }
        else
        {
            //sets power to 0 when nothing is supposed to happen
            climbArms.Set(0);
        }

        if (robotData.controllerData.sLYStick < -0.08 || robotData.controllerData.sLYStick > 0.08)
        {
            //sets arm power to joystick when joystick is outside of deadzone
            climbElevator.Set(-robotData.controllerData.sLYStick); //sets the power to 0 so the arms stop moving
        }
        else
        {
            //sets power to 0 when nothing is supposed to happen
            climbElevator.Set(0);
        }


}

void Climb::semiAuto(const RobotData &robotData, ClimbData &climbData)
{

    //listens for climb initiation button and does somthing if it needs to
    climbInit(robotData, climbData);
    //this is the climb sequence where the bot will climb autonomously
    runSequence(robotData, climbData);
    //this will cancel the sequence or pause it
    cancelSequence(robotData, climbData);
    
}

void Climb::climbInit(const RobotData &robotData, ClimbData &climbData)
{
    if (robotData.controllerData.sBBtn && !climbInitiating) //if the climbInit button is pressed then the climb will go up if the climb is down and go down if it is already up. (toggle)
    {
        climbUp = !climbUp; //sets the climbUp to the opposite direction
        climbInitiating = true; //sets the climbInitiating variable to true to 
    }

    if (climbInitiating && climbUp)
    {
        ChangeElevatorSpeed(1,0);
        RunElevatorToPos(140,0,0); //runs the climb up when button is pressed
    }
    else if (climbInitiating && !climbUp)
    { 
        ChangeElevatorSpeed(1,0);
        RunElevatorToPos(0,0,0); //runs the climb back down if you press it again
    }
}

void Climb::cancelSequence(const RobotData &robotData, ClimbData &climbData) //cancels the climb sequence
{


}

//runs climb sequence
void Climb::runSequence(const RobotData &robotData, ClimbData &climbData)
{

    //sets variables to go up to 3rd bar
    if (robotData.controllerData.sRCenterBtn)
    {//3rd bar
        //stage = 0;//for testing
        executeSequence = true; //press right center button, semiAuto code runs
        climbData.climbing = true;
        targetBar = 3; //reaches to bar 3
        ChangeElevatorSpeed(elevatorSpeed, 0);
    }//sets variables to go up to 4th bar
    else if (robotData.controllerData.sLCenterBtn)
    {//4th bar
        //stage = 0;//for testing
        executeSequence = true; //press left center button, semiAuto code runs
        climbData.climbing = true;
        targetBar = 4; //reaches to bar 4
        ChangeElevatorSpeed(elevatorSpeed, 0);
    }

    //starts going up to bar
    if (executeSequence && climbData.bar < 4)
    { //checks if you want to run the sequence, and also if you're already at bar 4, then you can't run it
        if (stage == 0) ChangeElevatorSpeed(elevatorSpeed, 1);
        else if (stage == 1) RunArmsAndElevatorToPos(-1,1,0,0,1); // get value in manual
        else if (stage == 2) ChangeElevatorSpeed(0.5, 1);
        else if (stage == 3) RunElevatorToPos(20,1,0); //test 3
        else if (stage == 4) ChangeElevatorSpeed(1,1);
        else if (stage == 5) RunArmsToPos(20,1,1);
        //top bar transfer
        if (climbData.bar == targetBar-1)
        {
            if (stage == 6) RunArmsAndElevatorToPos(120,0,65,1,1);
            else if (stage == 7) WaitUntilGyro(-1, 2, 32, 1);
            else if (stage == 8) RunElevatorToPos(146,1,1);
            else if (stage == 9) ChangeElevatorSpeed(elevatorSpeed,1);
            else if (stage == 10) ChangeArmSpeed(0.7,1);
            else if (stage == 11) TopTransfer();
            else if (stage == 12) {ChangeArmSpeed(1,1); lastTrasfer = false;}
            else if (stage == 13) RunArmsToPos(0,1,1);
            // else if (stage == 16) ChangeElevatorSpeed(0.6, 1);
            // else if (stage == 17) RunElevatorToPos(70,1,1);
            // else if (stage == 18) ChangeElevatorSpeed(elevatorSpeed, 1);
        }
        //transfer onto 3rd bar
        else 
        {
            if (stage == 6) RunArmsAndElevatorToPos(100,0,165,1,1);
            else if (stage == 7) WaitUntilGyro(1, -1, 42, 1);
            else if (stage == 8) RunElevatorToPos(150,1,1);
            else if (stage == 9) RunArmsToPos(100,1,1);
            else if (stage == 10) RunElevatorToPos(110,1,1);
            else if (stage == 11) ChangeElevatorSpeed(elevatorSpeed, 1);
        }
        if (stage == 12)
        { //do it again if the bot isnt on the top bar
            //resets everything
            stage = 0;
            //adds 1 to bar because the robot has gone up
            climbData.bar++;
            //if the bar is at its target then stop else keep going
            if (climbData.bar == targetBar)
            {
                executeSequence = false;
            }

            //resets powers
            climbArms.Set(0);
            climbElevator.Set(0);
        }
    }
}



void Climb::RunArmsGyroLower()
{

}

void Climb::RunArmsGyroUpper()
{
    
}

void Climb::wait(int time)
{
    timer += 1;
    if (timer > time)
    {
        timer = 0;
        stage += 1;
    }
}



//Runs the elevator to a specific location, specified in semiAuto
void Climb::RunElevatorToPos(int position, int stageAdd, int onBar)
{
    if (climbElevatorEncoder.GetPosition() > -position + 1.5 || climbElevatorEncoder.GetPosition() < -position - 1.5)
    {
        elevatorRunning = true;
        //only moves when angular rate is low to reduce swinging
        if (onBar==1){
            if (abs(angularRate) < 50)
            {
                climbElevator_pidController.SetReference(-position, rev::CANSparkMax::ControlType::kPosition, onBar);
            }
            else
            {
                climbElevator.Set(0);
                
            }
        } else{
            climbElevator_pidController.SetReference(-position, rev::CANSparkMax::ControlType::kPosition, onBar);
        }
        elevatorRunning = true;
        
    }
    else
    {
        elevatorRunning = false;
        climbInitiating = false;
        stage += stageAdd;
    }
}

//runs the arms to a specific location, specified in semiAuto
void Climb::RunArmsToPos(int position, int stageAdd, int onBar)
{

    if (climbArmsEncoder.GetPosition() > -position + 1.5 || climbArmsEncoder.GetPosition() < -position - 1.5)
    {
        armsRunning = true;
        climbArms_pidController.SetReference(-position, rev::CANSparkMax::ControlType::kPosition, onBar);
    }
    else
    {
        armsRunning = false;
        stage += stageAdd;
    }
}

//runs both elevator and arms
void Climb::RunArmsAndElevatorToPos(int elevatorPos, int elevatorBar, int armsPos, int armsBar, int stageAdd)
{
    RunElevatorToPos(elevatorPos, 0, elevatorBar);
    RunArmsToPos(armsPos, 0, armsBar);
    //waits until the arms and the elevator are done running
    if (!elevatorRunning && !armsRunning)
    {
        climbArms.Set(0);
        climbElevator.Set(0);
        stage+=stageAdd;
    }

}

//trasfers on top bar
void Climb::TopTransfer()
{

    //checks for angle where the bot pulls off bar
    if (angle > 44 || elevatorRunning)
    {
        climbArms.Set(0);
        ChangeElevatorSpeed(0.6, 0);
        RunElevatorToPos(110,1,0);
        lastTrasfer = true;
    } else if (!lastTrasfer){
        
        //runs arms down when not at the angle for transfer
        climbArms_pidController.SetReference(-200, rev::CANSparkMax::ControlType::kPosition, 1);
    }
}


//checks for a specific gyro value before moving on
void Climb::WaitUntilGyro(int cmp, int arate, float gyroValue, int stageAdd)
{
    //checks greater than a value
    if (cmp == 1)
    {
        if (gyroValue < angle )
        {
            if (arate == 1 && angularRate > 0)
            {
                stage += stageAdd;
            }
            else if (arate == -1 && angularRate < 0)
            {
                stage += stageAdd;
            }
            if (arate == 2)
            {
                stage += stageAdd;
            }
        }
    }
    //checks less than a value
    else if (cmp == -1)
    {
        if (gyroValue > angle)
        {
            if (arate == 1 && angularRate > 0)
            {
                stage += stageAdd;
            }
            else if (arate == -1 && angularRate < 0)
            {
                stage += stageAdd;
            }
            if (arate == 2)
            {
                stage += stageAdd;
            }
        }
    }
}

void Climb::ChangeElevatorSpeed(float speed, int stageAdd)
{
    //changes speed on elevator on bot PIDS
    climbElevator_pidController.SetOutputRange(-speed, speed, 0);
    climbElevator_pidController.SetOutputRange(-speed, speed, 1);
    stage += stageAdd;
}


void Climb::ChangeArmSpeed(float speed, int stageAdd)
{
    //changed speed on arms on both PIDS
    climbArms_pidController.SetOutputRange(-speed, speed, 0);
    climbArms_pidController.SetOutputRange(-speed, speed, 1);
    stage += stageAdd;
}

void Climb::updateData(const RobotData &robotData, ClimbData &climbData)
{
    
    // climbData.elevatorAmp = climbElevator.GetOutputCurrent();
    // climbData.armsAmp = climbArms.GetOutputCurrent();
    // climbData.elevatorTemp = climbElevator.GetMotorTemperature();
    // climbData.armsTemp = climbArms.GetMotorTemperature();
    // climbData.elevatorPos = climbElevatorEncoder.GetPosition();
    // climbData.armsPos = climbArmsEncoder.GetPosition();
    // climbData.armsAbsPos = climbArmsAbs.GetOutput();
    climbData.stage = stage;
    // climbData.angle = angle;
    // climbData.angularRate = angularRate;
    // climbData.elevatorLimit = elevatorLimit.Get();
    
    // frc::SmartDashboard::PutNumber("elevator amps", elevatorAmperage);
    // frc::smartDashboard::PutNumber("Arms amps", armsAmperage);
    // frc::SmartDashboard::PutNumber("climb stage", stage);
    // frc::smartDashboard::PutBoolean("running sequence", executeSequence);
    // frc::smartDashboard::PutNumber("which bar is bot on bar", climbData.bar);
    // frc::smartDashboard::PutBoolean("zeroing", climbData.zeroing);
    // frc::smartDashboard::PutNumber("elevator motor temp", elevatorTemp);
    // frc::smartDashboard::PutNumber("arms temp", armsTemp);
    // frc::SmartDashboard::PutNumber("climb angle", angle);
    // frc::SmartDashboard::PutNumber("climb angle rate", angularRate);
}



//sets powers to 0 when disabled
void Climb::DisabledInit()
{
    //sets motors to 0 for cuz disabled
    climbElevator.Set(0);
    climbArms.Set(0);
}


//runs update when disabled
void Climb::DisabledPeriodic(const RobotData &robotData, ClimbData &climbData)
{
    updateData(robotData, climbData);
}