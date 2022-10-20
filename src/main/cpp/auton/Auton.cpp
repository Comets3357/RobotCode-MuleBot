#include "RobotData.h"

void Auton::RobotInit(AutonData &autonData) {
    sendAutonSelectionChooser();
}

// creates pathGroup vector (list of strings that are interpretted by drivebase)
void Auton::AutonomousInit(AutonData &autonData)
{
    autonData.autonStep = -1;   // starts at -1 so that the stepper function can advance it to index 0 the first time

    // directory to deploy folder on roborio
    fs::path deployDirectory = frc::filesystem::GetDeployDirectory();

    autonData.autonRoutineName = autonChooser.GetSelected();
    fs::path autonDirectory = deployDirectory / "Autos" / autonData.autonRoutineName;
    // frc::SmartDashboard::PutString("autonDiredctory", autonDirectory.string());

    std::ifstream inFile;
    inFile.open(autonDirectory.string());

    autonData.pathGroup.clear();

    if (inFile.fail()) {
    //    frc::SmartDashboard::PutString("fail", "failed");
    } else {
        std::string str;
        while (getline(inFile, str)) {
            // frc::SmartDashboard::PutString("str", str);
            autonData.pathGroup.push_back(str);
        }
    }

    // remove newline char from all but the final line
    for (size_t i = 0; i < autonData.pathGroup.size(); i++) {
        std::string correctPathName = autonData.pathGroup[i];

        // frc::SmartDashboard::PutBoolean("int bool" + std::to_string(i), correctPathName[correctPathName.length() - 1] == 13);
        // frc::SmartDashboard::PutNumber(std::to_string(i) + "int", correctPathName[correctPathName.length() - 1]);

        // if the last char in the string is a newline, delete it for proper auton selection processing
        if (int(correctPathName[correctPathName.length() - 1]) == 13) {
            correctPathName = correctPathName.substr(0, correctPathName.length() - 1);  // get rid of hidden newline from file line read
        }
        
        autonData.pathGroup[i] = correctPathName;

        // frc::SmartDashboard::PutString(std::to_string(i), autonData.pathGroup[i]);
        
        
        // frc::SmartDashboard::PutString(std::to_string(i), autonData.pathGroup[i]);        
    }
}

void Auton::sendAutonSelectionChooser() {
    autonChooser.AddOption("potato", "potato");

    autonChooser.AddOption("taxiShootA", "taxiShootA");
    autonChooser.AddOption("taxiShootB", "taxiShootB");
    autonChooser.AddOption("taxiShootC", "taxiShootC");

    autonChooser.AddOption("fourBallMule", "fourBallMule");

    autonChooser.AddOption("citrus", "citrus");
    autonChooser.AddOption("nearFieldOne", "nearFieldOne");

    frc::SmartDashboard::PutData("Select Auton:", &autonChooser);
}


void Auton::AutonomousPeriodic(const RobotData &robotData, AutonData &autonData, ControlData &controlData, ControllerData &controllerData)
{
    // frc::smartDashboard::PutString("autonRoutineName", autonData.autonRoutineName);

    frc::SmartDashboard::PutNumber("auton step", autonData.autonStep);

    if (autonData.autonRoutineName == "potato")
    {
        potato(robotData, controlData, controllerData);
    }
    else if (autonData.autonRoutineName == "citrus") {
        citrus(robotData, controlData, controllerData);
    }
    else if (autonData.autonRoutineName == "nearFieldOne") {
        nearFieldOne(robotData, controlData, controllerData);
    }
    else if (autonData.autonRoutineName == "taxiShootA") {
        taxiShootA(robotData, controlData, controllerData);
    }
    else if (autonData.autonRoutineName == "taxiShootB") {
        taxiShoot(robotData, controlData, controllerData);
    }
    else if (autonData.autonRoutineName == "taxiShootC") {
        taxiShoot(robotData, controlData, controllerData);
    }
}


void Auton::potato(const RobotData &robotData, ControlData &controlData, ControllerData &controllerData)
{
    controlData.intake = false;
    controlData.shootMode = shootMode_none;
}


void Auton::citrus(const RobotData &robotData, ControlData &controlData, ControllerData &controllerData) {
    sec = robotData.timerData.secSinceEnabled;

    
}


void Auton::nearFieldOne(const RobotData &robotData, ControlData &controlData, ControllerData &controllerData) {
    sec = robotData.timerData.secSinceEnabled;

    if (sec > 0 && sec < 10) 
    {
        controlData.backShot = true;
    } 
    else 
    {
        controlData.backShot = false;
    }
}


void Auton::taxiShoot(const RobotData &robotData, ControlData &controlData, ControllerData &controllerData) {
    sec = robotData.timerData.secSinceEnabled;

    if (sec > 0 && sec < 3) 
    {
        controlData.backShot = true;
        controlData.shootMode == shootMode_none;
    }
    else 
    {
        controlData.backShot = false;
        controlData.shootMode == shootMode_none;
    }

    if (sec > 10 && sec < 15) 
    {
        controlData.shooting = true;
        controlData.shootMode = shootMode_vision;
    }
    else 
    {
        controlData.shooting = false;
        controlData.shootMode = shootMode_none;
    }
    // intake
    // if (sec > 0 && sec < 4) {
    //     controlData.intake= true;
    // } else {
    //     controlData.intake = false;
    // }

    // // shooting
    // if (sec > 3 && sec < 7) {
    //     controlData.shootMode = shootMode_vision;
    // } else {
    //     controlData.shootMode = shootMode_none;
    // }

    // if (sec > 5 && sec < 7) {
    //     controlData.shooting = true;
    // } else {
    //     controlData.shooting = false;
    // }
}

void Auton::taxiShootA(const RobotData &robotData, ControlData &controlData, ControllerData &controllerData) {
    sec = robotData.timerData.secSinceEnabled;

    if (sec > 0 && sec < 3) 
    {
        controlData.backShot = true;
    }
    else
    {
        controlData.backShot = false;
    }

    if (sec > 3 && sec < 6) 
    {
        controlData.intake = true;
    }
    else 
    {
        controlData.intake = false;
    }

    if (sec > 6 && sec < 9)
    {
        controlData.shootMode = shootMode_vision;
    }
    else 
    {
        controlData.shootMode = shootMode_vision;
    }

    // intake
    // if (sec > 0 && sec < 11) {
    //     controlData.intake = true;
    //     // controlData.saEjectBalls = false;
    // } else if (sec > 11) {
    //     controlData.intake = false;
    //     // controlData.saEjectBalls = true;
    // }

    // // shooting
    // controlData.shootMode = shootMode_vision;

    // if (sec > 1.5 && sec < 6) {
    //     controlData.shooting = true;
    // } else {
    //     controlData.shooting = false;
    // }
}






