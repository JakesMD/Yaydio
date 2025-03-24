#include <Adafruit_PN532.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

enum YNFCResult {
    NO_CARD,
    SUCCESS,
    FAIL,
};

class YNFC {
   public:
    YNFC(byte irqPin, byte resetPin) : _nfc(irqPin, resetPin) {};

    bool begin() { return _nfc.begin(); }

    YNFCResult read() {
        uint8_t success;
        uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
        uint8_t uidLength;                      // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

        // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
        // 'uid' will be populated with the UID, and uidLength will indicate
        // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
        success = _nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

        if (!success) return NO_CARD;
        if (uidLength != 4) return FAIL;

        _nfc.PrintHex(uid, uidLength);

        uint8_t keya[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

        // Start with block 4 (the first block of sector 1) since sector 0
        // contains the manufacturer data and it's probably better just
        // to leave it alone unless you know what you're doing
        success = _nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

        if (!success) return FAIL;

        uint8_t data[16];

        success = _nfc.mifareclassic_ReadDataBlock(4, data);

        if (!success) return FAIL;

        _nfc.PrintHexChar(data, 16);

        // Reconstruct album number from the first two bytes (little-endian)
        _album = data[0] | (data[1] << 8);

        return SUCCESS;
    }

    YNFCResult setAlbum(uint16_t album) {
        YNFCResult readResult = read();
        if (read() != SUCCESS) return readResult;

        uint8_t data[16] = {0};

        // Store the album number (16-bit) in the first two bytes (little-endian format)
        data[0] = album & 0xFF;         // Lower 8 bits
        data[1] = (album >> 8) & 0xFF;  // Upper 8 bits

        if (_nfc.mifareclassic_WriteDataBlock(4, data)) {
            _album = album;
            return SUCCESS;
        }
        return FAIL;
    }

    uint16_t album() { return _album; }

   private:
    uint16_t _album = 0;
    Adafruit_PN532 _nfc;
};