#include <TM1637Display.h>

const uint8_t _fail[] = {
    SEG_A | SEG_E | SEG_F | SEG_G,                  // F
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  // A
    SEG_B | SEG_C,                                  // |
    SEG_D | SEG_E | SEG_F,                          // L
};

const uint8_t _play[] = {
    SEG_C | SEG_E,  // ||
};

const uint8_t _stop[] = {
    SEG_C | SEG_D | SEG_E | SEG_G,  // []
};

const uint8_t _blank[] = {
    SEG_G,  // -
    SEG_G,  // -
    SEG_G,  // -
    SEG_G,  // -
};

const uint8_t _loading[] = {
    SEG_D | SEG_E | SEG_F,                          // L
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  // A
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
};

const uint8_t _hi[] = {
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  // H
    SEG_B | SEG_C,                          // |
};

class YDisplay {
   public:
    YDisplay(byte clkPin, byte dioPin) : _display(clkPin, dioPin) { _display.setBrightness(3); }

    void clear() { _display.clear(); }

    void showTrack(uint16_t track, bool isPlaying) {
        _display.setSegments(isPlaying ? _play : _stop, 1);
        _display.showNumberDec(track, false, 3, 1);
    }

    void showAlbum(uint16_t album) { _display.showNumberDec(album, true, 4, 0); }

    void showFail() { _display.setSegments(_fail); }

    void showBlank() { _display.setSegments(_blank); }

    void showLoading() { _display.setSegments(_loading); }

    void showHi() {
        _display.clear();
        _display.setSegments(_hi, 2, 1);
    }

   private:
    TM1637Display _display;
};