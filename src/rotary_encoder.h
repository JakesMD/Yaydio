#include <ezButton.h>

enum YRotaryEncoderDirection { CCW, NONE, CW };

class YRotaryEncoder {
   public:
    YRotaryEncoder(byte clkPin, byte dtPin) : _clkBtn(clkPin), _dtBtn(dtPin) { _clkBtn.setDebounceTime(0); }

    void loop() {
        _clkBtn.loop();
        _currentCLKState = _clkBtn.getState();

        if (_currentCLKState == LOW && _currentCLKState != _previousCLKState) {
            _direction = _dtBtn.getStateRaw() ? CW : CCW;
        } else {
            _direction = NONE;
        }

        _previousCLKState = _currentCLKState;
    }

    YRotaryEncoderDirection getDirection() { return _direction; }

    bool justTurnedCW() { return _direction == CW; }

    bool justTurnedCCW() { return _direction == CCW; }

   private:
    ezButton _clkBtn;
    ezButton _dtBtn;
    YRotaryEncoderDirection _direction;
    bool _currentCLKState = HIGH;
    bool _previousCLKState = HIGH;
};