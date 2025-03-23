#include <Arduino.h>
#include <DYPlayerArduino.h>
#include <SoftwareSerial.h>

class YMP3Player {
   public:
    YMP3Player(byte rxPin, byte txPin) : _serial(rxPin, txPin), _player(&_serial) {}

    bool begin() {
        _player.begin();
        _player.setVolume(15);
        _player.setCycleMode(DY::play_mode_t::RepeatDir);
        _player.stop();

        return !(_player.checkPlayState() == DY::play_state_t::Fail);
    }

    void playAlbum(uint16_t album) {
        char path[12];
        sprintf(path, "/%04d/001.mp3", album);

        _player.stop();
        _player.playSpecifiedDevicePath(DY::device_t::Sd, path);
        _isPlaying = true;
        delay(1000);
        _firstTrackInAlbum = _player.getFirstInDir();
    }

    void togglePlayback() {
        _isPlaying = !_isPlaying;

        if (_isPlaying) {
            _player.play();
        } else {
            _player.pause();
        }
    }

    void increaseVolume() { _player.volumeIncrease(); }

    void decreaseVolume() { _player.volumeDecrease(); }

    void next() {
        _player.next();
        _isPlaying = true;
    }

    void previous() {
        _player.previous();
        _isPlaying = true;
    }

    void stop() {
        _player.stop();
        _isPlaying = false;
    }

    bool isPlaying() { return _isPlaying; }

    uint16_t currentTrack() { return _player.getPlayingSound() - _firstTrackInAlbum + 1; }

      private:
    SoftwareSerial _serial;
    DY::Player _player;
    uint16_t _firstTrackInAlbum;
    bool _isPlaying = false;
};