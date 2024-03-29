#include "vex.h"

using namespace vex;
competition Competition;

Drive chassis(
TANK_ONE_ROTATION,
motor_group(BLMotor, FLMotor, MLMotor),
motor_group(BRMotor, FRMotor, MRMotor),
PORT16,
3.08,
2.5,
360,
PORT1,     -PORT2,
PORT3,     -PORT4,
PORT5,
2.75,
1,
PORT5,
2.75,
5.2
);

int current_auton_selection = 0;
bool auto_started = false;

bool isRWingOpen = false;
bool isLWingOpen = false;
bool prevR2State = false;

void toggleRWing() {
  isRWingOpen = !isRWingOpen;
  RWing.set(isRWingOpen);
}

void toggleLWing() {
  isLWingOpen = !isLWingOpen;
  LWing.set(isLWingOpen);
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  default_constants();

  while(auto_started == false){            
    Brain.Screen.clearScreen();           
    switch(current_auton_selection){      
      case 0:
        Brain.Screen.printAt(50, 50, "Descore Winpoint");
        break;
      case 1:
        Brain.Screen.printAt(50, 50, "2 PT Push + Lineup");
        break;
      case 2:
        Brain.Screen.printAt(50, 50, "5 Point Push(NO BAR TOUCH)");
        break;
      case 3:
        Brain.Screen.printAt(50, 50, "WinPoint(5 POINT PUSH + BAR TOUCH)");
        break;

    }
    if(Brain.Screen.pressing()){
      while(Brain.Screen.pressing()) {}
      current_auton_selection++;
    } else if (current_auton_selection == 4){
      current_auton_selection = 0;
    }
    task::sleep(10);
  }
}

void autonomous(void) {
  auto_started = true;
  switch(current_auton_selection){  
    case 0:
      descore_winpoint();
      break;        
    case 1:         
      push();
      break;
    case 2:
      push_bad();
      break; 
    case 3:
      winpoint();
  } 
}

void usercontrol(void) {
  chassis.set_coordinates(0,0,0);
  while (1) {
    default_constants();
    chassis.control_arcade();

    bool currentR2State = Controller.ButtonR2.pressing();

    if(currentR2State && !prevR2State) {
      toggleRWing();
      toggleLWing();
    }

    prevR2State = currentR2State; 

    if(Controller.ButtonR1.pressing()) {
      Slapper.spin(forward, 15, volt);
    } else {
      Slapper.spin(forward, 0, volt);
    }

    wait(20, msec);
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function
  pre_auton();

  // Prevent main from exiting with an infinite loop
  while (true) {
    wait(100, msec);
  }
}

//END 23-24 SEASON