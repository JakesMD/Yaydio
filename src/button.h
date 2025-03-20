#include <ezButton.h>

enum YButtonState { JUST_PRESSED, STILL_PRESSED, JUST_RELEASED, STILL_RELEASED };

class YButton {
   public:
    YButton(byte pin) : _btn(pin) { _btn.setDebounceTime(20); }

    void loop() {
        _btn.loop();
        _currentState = _btn.getState();

        if (_currentState != _previousState) {
            _state = _currentState == LOW ? JUST_PRESSED : JUST_RELEASED;
        } else {
            _state = _currentState == LOW ? STILL_PRESSED : STILL_RELEASED;
        }

        _previousState = _currentState;
    }

    bool justPressed() { return _state == JUST_PRESSED; }
    bool justReleased() { return _state == JUST_RELEASED; }
    bool isPressed() { return _state == STILL_PRESSED || _state == JUST_PRESSED; }
    bool isReleased() { return _state == STILL_RELEASED || _state == JUST_RELEASED; }

    YButtonState getState() { return _state; }

   private:
    ezButton _btn;
    YButtonState _state;
    bool _currentState = HIGH;
    bool _previousState = HIGH;
};