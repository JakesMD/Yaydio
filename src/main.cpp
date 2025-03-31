#include <Arduino.h>
#include <TM1637Display.h>

#include "button.h"
#include "config.h"
#include "display.h"
#include "mp3_player.h"
#include "nfc.h"
#include "rotary_encoder.h"

YButton cardBtn(CARD_BTN_PIN);

YButton leftBtn(LEFT_BTN_PIN);
YButton rightBtn(RIGHT_BTN_PIN);

YRotaryEncoder leftRotaryEncoder(LEFT_CLK_PIN, LEFT_DT_PIN);
YRotaryEncoder rightRotaryEncoder(RIGHT_CLK_PIN, RIGHT_DT_PIN);

YDisplay display(DISP_CLK_PIN, DISP_DIO_PIN);

YNFC nfc(NFC_IRQ_PIN, NFC_RST_PIN);

YMP3Player mp3Player(MP3_RX_PIN, MP3_TX_PIN);

enum YMODE {
    NO_CARD_MODE,
    READ_CARD_MODE,
    WRITE_CARD_MODE,
    PLAYER_MODE,
    SET_ALBUM_MODE,
    NO_ALBUM_MODE,
    FAIL_MODE,
};

YMODE mode = NO_CARD_MODE;
bool hasModeChanged = true;
bool isNewModeComing = false;

bool wasRightBtnReleased = false;

unsigned long lastUpdate = 0;
unsigned long currentMillis = 0;

uint8_t selectedAlbum = 1;

void updateTrackScreen() {
    display.showTrack(mp3Player.currentTrack(), mp3Player.isPlaying());
    lastUpdate = currentMillis;
}

void changeMode(YMODE newMode) {
    mode = newMode;
    isNewModeComing = true;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Hi!");

    if (!nfc.begin() || !mp3Player.begin()) {
        display.showFail();
        while (true);
    }
}

void loop() {
    cardBtn.loop();
    leftBtn.loop();
    rightBtn.loop();
    leftRotaryEncoder.loop();
    rightRotaryEncoder.loop();

    currentMillis = millis();

    switch (mode) {
        case NO_CARD_MODE:
            if (hasModeChanged) {
                mp3Player.stop();
                display.showHi();
            }
            if (cardBtn.isPressed()) changeMode(READ_CARD_MODE);
            break;

        case READ_CARD_MODE:
            if (hasModeChanged) {
                display.showLoading();
                mp3Player.stop();
            }
            switch (nfc.read()) {
                case NO_CARD:
                    changeMode(NO_CARD_MODE);
                    break;
                case SUCCESS:
                    selectedAlbum = nfc.album();
                    changeMode(selectedAlbum == 0 ? NO_ALBUM_MODE : PLAYER_MODE);
                    break;
                case FAIL:
                    changeMode(FAIL_MODE);
                    break;
            }
            break;

        case WRITE_CARD_MODE:
            if (hasModeChanged) display.showLoading();
            switch (nfc.setAlbum(selectedAlbum)) {
                case NO_CARD:
                    changeMode(NO_CARD_MODE);
                    break;
                case SUCCESS:
                    if (selectedAlbum == 0) {
                        changeMode(NO_ALBUM_MODE);
                    } else {
                        changeMode(PLAYER_MODE);
                    }
                    break;
                case FAIL:
                    changeMode(FAIL_MODE);
                    break;
            }
            break;

        case PLAYER_MODE:
            if (hasModeChanged) {
                mp3Player.playAlbum(selectedAlbum);
                updateTrackScreen();
            }

            if (cardBtn.isReleased()) {
                changeMode(NO_CARD_MODE);
                break;
            }

            if (leftBtn.justReleased() || rightBtn.justReleased()) {
                mp3Player.togglePlayback();
                updateTrackScreen();
            }

            if (leftBtn.isPressed() && rightBtn.isPressed()) {
                changeMode(SET_ALBUM_MODE);
                break;
            }

            if (leftRotaryEncoder.justTurnedCW()) {
                mp3Player.increaseVolume();
            } else if (leftRotaryEncoder.justTurnedCCW()) {
                mp3Player.decreaseVolume();
            }

            if (rightRotaryEncoder.justTurnedCW()) {
                mp3Player.next();
                updateTrackScreen();
            } else if (rightRotaryEncoder.justTurnedCCW()) {
                mp3Player.previous();
                updateTrackScreen();
            }

            if (currentMillis - lastUpdate > 1000) updateTrackScreen();

            break;

        case SET_ALBUM_MODE:
            if (hasModeChanged) {
                mp3Player.stop();
                wasRightBtnReleased = false;
                if (selectedAlbum == 0) selectedAlbum = 1;
                display.showAlbum(selectedAlbum);
            }

            if (cardBtn.isReleased()) {
                changeMode(NO_CARD_MODE);
                break;
            }

            if (rightBtn.justReleased()) {
                if (wasRightBtnReleased) {
                    changeMode(WRITE_CARD_MODE);
                } else {
                    wasRightBtnReleased = true;
                }

            } else if (selectedAlbum < 9999 && rightRotaryEncoder.justTurnedCW()) {
                selectedAlbum++;
                display.showAlbum(selectedAlbum);
            } else if (selectedAlbum > 0 && rightRotaryEncoder.justTurnedCCW()) {
                selectedAlbum--;
                display.showAlbum(selectedAlbum);
            }

            break;

        case NO_ALBUM_MODE:
            if (hasModeChanged) {
                mp3Player.stop();
                display.showBlank();
            }
            if (cardBtn.isReleased()) {
                changeMode(NO_CARD_MODE);
                break;
            }
            if (leftBtn.isPressed() && rightBtn.isPressed()) {
                changeMode(SET_ALBUM_MODE);
                break;
            }
            if (rightBtn.justReleased()) {
                changeMode(READ_CARD_MODE);
                break;
            }
            break;

        case FAIL_MODE:
            if (hasModeChanged) {
                lastUpdate = currentMillis;
                display.showFail();
                mp3Player.stop();
            } else if (currentMillis - lastUpdate > 2000) {
                changeMode(NO_CARD_MODE);
            }
            break;
    }

    if (isNewModeComing) {
        hasModeChanged = true;
        isNewModeComing = false;
    } else {
        hasModeChanged = false;
    }
}
