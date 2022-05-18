#pragma once

enum Buttons {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    A = 4,
    B = 5,
    SELECT = 6,
    START = 7
};

class ControlManager {
    public:
        void get_button_states();
    private:
        bool button_pressed[8];
};