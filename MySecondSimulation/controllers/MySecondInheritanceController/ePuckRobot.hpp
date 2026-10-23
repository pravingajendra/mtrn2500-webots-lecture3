#pragma once

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

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

    std::array<webots::DistanceSensor*, N_DIST_SENSORS> mPs {};
    std::array<double, N_DIST_SENSORS> mPsValues {};
    bool mLeftObstacle {};
    bool mRightObstacle {};

    webots::Motor* mLeftMotor {};
    webots::Motor* mRightMotor {};
    double mLeftSpeed {};
    double mRightSpeed {};
  private:
};