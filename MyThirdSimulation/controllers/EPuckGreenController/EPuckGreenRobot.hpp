#pragma once

#include "EPuckRobot.hpp"

#include <webots/Keyboard.hpp>


class EPuckGreenRobot : public EPuckRobot {
public:
    EPuckGreenRobot();
    void run();
protected:
    void display() const;
    void turn();
private:
    webots::Keyboard* mKeyboard {};
};