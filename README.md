# TomatoCube IR & Magnet Polarity Tester

## (a) Description

The TomatoCube IR & Magnet Polarity Tester is a handheld test tool built around Microchip's ATtiny1614 microcontroller with a 16x2 HD44780 LCD. Point an infrared remote at its IR sensor and any received IR datagram is shown on screen; press the button for secondary details on the received datagram. Bring a magnet near the Hall effect sensor and the tester displays which pole, North or South, is facing it. The firmware is built in Arduino using megaTinyCore.

<img src="Images/TomatoCube IR & Magnet Polarity Tester.png" width="700" height="auto"/>



## (b) Instructions

1. Power on the tester. The LCD shows the ready screen.
2. **Testing an IR remote**
   * Point the remote at the tester's IR sensor and press any button on the remote.
   * The received IR datagram is shown on the LCD.
   * Press the tester's button to switch to the secondary information for that datagram.
3. **Checking magnet polarity**
   * Bring one face of the magnet close to the Hall effect sensor.
   * The LCD shows which pole (North or South) is facing the sensor.
   * Flip the magnet to check the other face.

## (c) Firmware

The firmware is an Arduino sketch built with [SpenceKonde](https://github.com/SpenceKonde)'s [megaTinyCore](https://github.com/SpenceKonde/megaTinyCore) and uploaded over UPDI.

1. **Install megaTinyCore** in the Arduino IDE:
   * Go to **File → Preferences** and add this URL to **Additional Boards Manager URLs**:
     ```
     http://drazzy.com/package_drazzy.com_index.json
     ```
   * Go to **Tools → Board → Boards Manager**, search for **megaTinyCore** and click **Install**.

2. **Open the firmware** sketch (`.ino`) in the Arduino IDE.
     ```
     /software/ir_magnet_tester/ir_magnet_tester.ino
     ```

3. **Select the board settings** under **Tools**:

   * Board: **ATtiny3224/1624/1614/1604/824/814/804/424/414/404/214/204**
   * Chip: **ATtiny1614**
   * Clock: **20 Mhz**
   * millis()/micros(): **TCD0 (1 series only, default there)**
   * Programmer: **SerialUPDI - 230400 baud**

4. **Connect a UPDI programmer** (e.g. a USB-serial adapter wired for SerialUPDI) to the tester's UPDI, GND and VCC pins.

5. Choose **Sketch → Upload Using Programmer**.

6. When the upload finishes, the tester restarts and shows the ready screen.

     

## (d) Showcase
<img src="Images/Front.jpg" width="700" height="auto"/>

<img src="Images/Back.jpg" width="700" height="auto"/>
