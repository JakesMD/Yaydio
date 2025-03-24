# Yaydio

## Limitations

- Supports MP3 format only.
- Max 999 songs per album.
- Max 9999 albums.
- Max 65535 songs in total.
- Max 32 GB SD card.


## Adding Albums to the SD Card

1.  If you haven't already, format the SD card to FAT16 or FAT32 and name it `YAYDIO`.
2.  Create a new folder named `0001`, where "1" represents the next album number.
3.  Copy the album's songs into the folder you created.
4.  Rename the first song of the album to `001.mp3`.
5.  Repeat steps 2-4 for each album.
6.  Use [DriveSort](https://www.anerty.net/software/file/DriveSort/?lang=en) (Windows) or [FatDriveSorter](https://fat-drive-sorter.netlify.app) (macOS) to sort the files alphabetically in the SD card's table.


## Assigning an Album to an NFC Card

1. Insert the NFC card into the Yaydio.
2. Press the left and right knobs simultaneously.
3. Turn the right knob to select the album number.
4. Press the right knob to write the album number to the NFC card.


## Wiring

| Left Rotary Encoder | Arduino Nano |
| ------------------- | ------------ |
| SW                  | D2           |
| SW GND              | GND          |
| DT                  | D3           |
| CLK                 | D4           |
| GND                 | GND          |

| Right Rotary Encoder | Arduino Nano |
| -------------------- | ------------ |
| SW                   | D5           |
| SW GND               | GND          |
| DT                   | D6           |
| CLK                  | D7           |
| GND                  | GND          |

| 4-Digit 7-Segment 0.36 Inch TM1637 | Arduino Nano |
| ---------------------------------- | ------------ |
| CLK                                | D10          |
| DIO                                | D11          |
| GND                                | GND          |
| 5V                                 | 5V           |

| Card Switch | Arduino Nano |
| ----------- | ------------ |
| NO          | A3           |
| COM         | GND          |

| PN532 NFC Module V3 | Arduino Nano |
| ------------------- | ------------ |
| GND                 | GND          |
| VCC                 | VIN (NOT 5V) |
| SDA                 | A4           |
| SCL                 | A5           |
| RSTPDN              | D9           |

| DY-SV5W Voice Module  | Arduino Nano |
| --------------------- | ------------ |
| -                     | GND          |
| +                     | 5V           |
| TXD                   | D12          |
| RXD                   | D13          |

| DY-SV5W Voice Module  | Speaker      |
| --------------------- | ------------ |
| Speaker +             | +            |
| Speaker -             | -            |
