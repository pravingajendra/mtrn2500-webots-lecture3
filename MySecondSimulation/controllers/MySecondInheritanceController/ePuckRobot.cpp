#include "ePuckRobot.hpp"

#include <string>
#include <iostream>
#include <iomanip>

EPuckRobot::EPuckRobot() 
  : mLeftMotor {getMotor("left wheel motor")}
  , mRightMotor {getMotor("right wheel motor")}
{
  // Initialising Dist Sensors
  const std::array<std::string, N_DIST_SENSORS> psNames {
    "ps0", "ps1", "ps2", "ps3",
    "ps4", "ps5", "ps6", "ps7"
  };
  for (int i {0}; i < N_DIST_SENSORS; ++i) {
    mPs[i] = getDistanceSensor(psNames[i]);
    mPs[i]->enable(TIME_STEP);
  }

  // Initialising Motors
  mLeftMotor->setPosition(INFINITY);
  mRightMotor->setPosition(INFINITY);
  mLeftMotor->setVelocity(0.0);
  mRightMotor->setVelocity(0.0);
}

void EPuckRobot::run() {
  // feedback loop: step simulation until an exit event is received
  while(step(TIME_STEP) != -1) {
    roam();
    report();
  }
}

void EPuckRobot::roam() {
  // read sensors outputs
  for(int i {0}; i < N_DIST_SENSORS; ++i) {
    mPsValues[i] = mPs[i]->getValue();
  }

  // detect obstacles
  const double obstacleThreshold {80.0};
  mRightObstacle = (
    mPsValues[0] > obstacleThreshold ||
    mPsValues[1] > obstacleThreshold ||
    mPsValues[2] > obstacleThreshold
  );
  mLeftObstacle = (
    mPsValues[5] > obstacleThreshold ||
    mPsValues[6] > obstacleThreshold ||
    mPsValues[7] > obstacleThreshold
  );

  // initialize motor speeds at 50% of MAX_SPEED.
  const double speedScale {0.5};
  double leftSpeed {speedScale * MAX_SPEED};
  double rightSpeed {speedScale * MAX_SPEED};
  
  // modify speeds according to obstacles
  if(mLeftObstacle) {
    // turn right
    leftSpeed  = speedScale * MAX_SPEED;
    rightSpeed = -speedScale * MAX_SPEED;
  } else if(mRightObstacle) {
    // turn left
    leftSpeed  = -speedScale * MAX_SPEED;
    rightSpeed = speedScale * MAX_SPEED;
  }

  // write actuators inputs
  mLeftMotor->setVelocity(leftSpeed);
  mRightMotor->setVelocity(rightSpeed);

}

void EPuckRobot::report() const {
  // for debugging, you can print the sensor readings to the console
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Time: " << getTime() << ' ';
  std::cout << "DistanceSensors: ";
  for(const auto elem : mPsValues) {
    std::cout << elem << ' ';
  }

  // for debugging, you can print the detection results to the console
  std::cout << "leftObstacle: " << mLeftObstacle << ' '
            << "rightObstacle: " << mRightObstacle << ' ';
  
  
  // for debugging, you can print the speeds to the console
  std::cout << "leftSpeed: " << mLeftSpeed << ' '
            << "rightSpeed: " << mRightSpeed << std::endl;
}

