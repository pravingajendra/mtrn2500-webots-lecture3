// File:          EPuckRobot.hpp
// Date:          XX/XX/XXXX
// Description:   Example in Lecture W.3
// Author:        Leo Wu
// Modifications:

#pragma once

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Emitter.hpp>
#include <webots/Receiver.hpp>

#include <array>

const int TIME_STEP {64};
const double MAX_SPEED {6.28};
const int N_DIST_SENSORS {8};

class EPuckRobot : public webots::Robot {
public:
  EPuckRobot();
  void run();
  
protected:
  void report() const;
  void roam();
  
  void sendMessage(const std::string& data);
  std::string receiveMessage();
  
  std::array<webots::DistanceSensor*, N_DIST_SENSORS> mPs {};
  std::array<double, N_DIST_SENSORS> mPsValues {};
  bool mLeftObstacle {};
  bool mRightObstacle {};
  
  webots::Motor* mLeftMotor {};
  webots::Motor* mRightMotor {};
  double mLeftSpeed {};
  double mRightSpeed {};  

private:  
  webots::Emitter* mEmitter {};
  webots::Receiver* mReceiver {};
};