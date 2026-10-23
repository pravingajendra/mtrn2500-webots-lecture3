#include "EPuckGreenRobot.hpp"


EPuckGreenRobot::EPuckGreenRobot() : mKeyboard{getKeyboard()} {
    mKeyboard->enable(TIME_STEP);
}

void EPuckGreenRobot::display() const {
    std::cout << "[EPuckGreenRobot]: Press and hold t/T and I'll turn myself." << std::endl;
    std::cout << "[EPuckGreenRobot]: Press b/B and I'll ask EPuckBlackRobot to move backwards." 
        << std::endl;
    std::cout << "[EPuckGreenRobot]: Press r/R and I'll ask EPuckBlackRobot to roam." << std::endl;

};

void EPuckGreenRobot::turn() {
    // initialize motor speeds at 50% of MAX_SPEED.
    const double speedScale {0.5};
    mLeftSpeed = speedScale * MAX_SPEED;
    mRightSpeed = speedScale * MAX_SPEED;

    // turn right
    mLeftSpeed  = speedScale * MAX_SPEED;
    mRightSpeed = -speedScale * MAX_SPEED;
    
    // write actuators inputs
    mLeftMotor->setVelocity(mLeftSpeed);
    mRightMotor->setVelocity(mRightSpeed);  
}

void EPuckGreenRobot::run() {
    display();
    while(step(TIME_STEP) != -1) {
        int k{mKeyboard->getKey()};
        if (k == 'T') {
            turn();
        } else {
            roam();
        }
    }
}