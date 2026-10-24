#include "EPuckBlackRobot.hpp"

EPuckBlackRobot::EPuckBlackRobot() : mState {State::roam} {}

void EPuckBlackRobot::backward() {
    // initialize motor speeds at 50% of MAX_SPEED.
    const double speedScale {0.5};
    mLeftSpeed = -speedScale * MAX_SPEED;
    mRightSpeed = -speedScale * MAX_SPEED;
    
    // write actuators inputs
    mLeftMotor->setVelocity(mLeftSpeed);
    mRightMotor->setVelocity(mRightSpeed); 
}

void EPuckBlackRobot::run() {
    while (step(TIME_STEP) != -1) {
        std::string msg {receiveMessage()};
        if (msg == "Roam") {
            // change state to roam
            mState = State::roam;
        } else if (msg == "Backward") {
            // change state to backward
            mState = State::backward;
        }

        if (mState == State::roam) {
            roam();
        } else if (mState == State::backward) {
            backward();
        }
    }
}