// File:          EPuckRobot.cpp
// Date:          XX/XX/XXXX
// Description:   Example in Lecture W.3
// Author:        Leo Wu
// Modifications:

#include "EPuckRobot.hpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <cstring>

EPuckRobot::EPuckRobot() 
  : mLeftMotor {getMotor("left wheel motor")} 
  , mRightMotor {getMotor("right wheel motor")}
  , mEmitter {getEmitter("emitter")}
  , mReceiver {getReceiver("receiver")}
  {
  
    // initialise distance sensors
    const std::array<std::string, N_DIST_SENSORS> psNames {
      "ps0", "ps1", "ps2", "ps3",
      "ps4", "ps5", "ps6", "ps7"
    };
    for (int i {0}; i < N_DIST_SENSORS; ++i) {
      mPs[i] = getDistanceSensor(psNames[i]);
      mPs[i]->enable(TIME_STEP);
    }
    
    // initialise motors
    mLeftMotor->setPosition(INFINITY);
    mRightMotor->setPosition(INFINITY);
    mLeftMotor->setVelocity(0.0);
    mRightMotor->setVelocity(0.0);
    
    // initialise receiver
    mReceiver->enable(TIME_STEP);
}

void EPuckRobot::report() const {
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Time: " << getTime() << ' ';
  std::cout << "DistanceSensors: ";
  for (const auto elem : mPsValues) {
    std::cout << elem << ' ';
  }
  std::cout << "leftObstacle: " << mLeftObstacle << ' '
            << "rightObstacle: " << mRightObstacle << ' ';
  std::cout << "leftSpeed: " << mLeftSpeed << ' '
            << "rightSpeed: " << mRightSpeed << std::endl;
}

void EPuckRobot::roam() {
  // read sensors outputs
  for (int i = 0; i < N_DIST_SENSORS ; ++i) {
    mPsValues[i] = mPs[i]->getValue();
  }
  
  // detect obstacles
  const double obstacleThreshold {80.0};
  mRightObstacle = {
    mPsValues[0] > obstacleThreshold ||
    mPsValues[1] > obstacleThreshold ||
    mPsValues[2] > obstacleThreshold
  };
  mLeftObstacle = {
    mPsValues[5] > obstacleThreshold ||
    mPsValues[6] > obstacleThreshold ||
    mPsValues[7] > obstacleThreshold
  };

  // initialize motor speeds at 50% of MAX_SPEED.
  const double speedScale {0.5};
  mLeftSpeed = speedScale * MAX_SPEED;
  mRightSpeed = speedScale * MAX_SPEED;
  // modify speeds according to obstacles
  if(mLeftObstacle) {
    // turn right
    mLeftSpeed  = speedScale * MAX_SPEED;
    mRightSpeed = -speedScale * MAX_SPEED;
  } else if(mRightObstacle) {
    // turn left
    mLeftSpeed  = -speedScale * MAX_SPEED;
    mRightSpeed = speedScale * MAX_SPEED;
  }
  // write actuators inputs
  mLeftMotor->setVelocity(mLeftSpeed);
  mRightMotor->setVelocity(mRightSpeed);  
}

void EPuckRobot::run() {
  // feedback loop: step simulation until an exit event is received
  while(step(TIME_STEP) != -1) {
    roam();
    // report();
  }
}

void EPuckRobot::sendMessage(const std::string& data) {
  mEmitter->send(data.c_str(), static_cast<int>(strlen(data.c_str())) + 1);
}

std::string EPuckRobot::receiveMessage() {
  if (mReceiver->getQueueLength() > 0) {
    std::string message {static_cast<const char *>(mReceiver->getData())};
    mReceiver->nextPacket();

    return message;
  }
  return "";
}