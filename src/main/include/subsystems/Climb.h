#pragma once

#include "Constants.h"

#include <frc/DriverStation.h>
#include <frc/TimedRobot.h>
#include <ctre/Phoenix.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxPIDController.h>
#include <rev/CANEncoder.h>
#include <rev/CANDigitalInput.h>
#include <frc/DigitalInput.h>
#include <frc/DutyCycle.h>
#include <frc/smartdashboard/SmartDashboard.h>


struct RobotData;

struct ClimbData {
    int bar = 2;
    int elevatorEncoderPosition = 0;
    bool climbing = false;
    bool zeroing = false;
    int stage;
    int angle, angularRate;
    bool elevatorLimit;

    // float armsAmp, elevatorAmp, armsTemp, elevatorTemp, elevatorPos, armsPos, armsAbsPos;

    //bench test
    float benchTestClimbArmsSpeed = 0;
    float benchTestClimbElevatorSpeed = 0;
    bool limitSwitchWorking = false;
    bool elevatorUpperLimit = false;
    bool elevatorLowerLimit = false;
    bool armsUpperLimit = false;
    bool armsLowerLimit = false;

    
};

class Climb {

public:
    void RobotInit();
    void RobotPeriodic(const RobotData &robotData, ClimbData &climbData);  
    void DisabledPeriodic(const RobotData &robotData, ClimbData &climbData); 
    void DisabledInit();
    void TestPeriodic(const RobotData &robotData, ClimbData &climbData);
    void TestInit(ClimbData &climbData);

private:

    float elevatorSpeed = 0.8;
    float armsSpeed = 1;
    int zeroingTimer = 0;

    void climbInit(const RobotData &robotData, ClimbData &climbData);
    void cancelSequence(const RobotData &robotData, ClimbData &climbData);
    void runSequence(const RobotData &robotData, ClimbData &climbData);

    int stage = 0;

    float angularRate = 0;
    float angle = 0;

    bool climbInitiating = false;
    bool climbUp = false;
    bool executeSequence = false;
    int targetBar = 0;

    bool elevatorDirection; //True is positive, False is negative
    bool elevatorRunning = false;

    bool armsDirection; //True is positive, False is negative
    bool armsRunning = false;

    int timer = 0;

    bool lastTrasfer = false;

    void updateData(const RobotData &robotData, ClimbData &climbData);
    void semiAuto(const RobotData &robotData, ClimbData &climbData);
    void manual(const RobotData &robotData, ClimbData &climbData);

    void RunElevatorToPos(int position, int stageAdd, int onBar);
    void RunArmsToPos(int position, int stageAdd, int onBar);
    void RunArmsAndElevatorToPos(int elevatorPos, int elevatorBar, int armsPos, int armsBar, int stageAdd);
    void ZeroElevator(float power, int stageAdd);
    void wait(int time);

    void ChangeElevatorSpeed(float speed, int stageAdd);
    void ChangeArmSpeed(float speed, int stageAdd);

    void WaitUntilGyro(int cmp, int arate, float gyroValue, int stageAdd);

    void CheckArms();
    void WaitUntilArmsOnBar();

    void TopTransfer();

    void RunArmsGyroLower();
    void RunArmsGyroUpper();

    //CHANGE MOTOr ID STUFF  (just outline lol don't take your life too seriously:))
    //initualizes climb elevator motor
    rev::CANSparkMax climbElevator = rev::CANSparkMax(elevatorMotorID, rev::CANSparkMax::MotorType::kBrushless);
    rev::SparkMaxRelativeEncoder climbElevatorEncoder = climbElevator.GetEncoder();
    rev::SparkMaxPIDController climbElevator_pidController = climbElevator.GetPIDController();

    //initualizes climb arms motor (i dont know if there are 2 motors yet)
    rev::CANSparkMax climbArms = rev::CANSparkMax(elevatorPivotMotorID, rev::CANSparkMax::MotorType::kBrushless);
    rev::SparkMaxRelativeEncoder climbArmsEncoder = climbArms.GetEncoder();
    rev::SparkMaxPIDController climbArms_pidController = climbArms.GetPIDController();

   
};