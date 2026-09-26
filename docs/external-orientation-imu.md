# Using an Adafruit BNO055 IMU to control listener orientation

Optional hardware add-on: drive the listener's orientation in 3D Audio
Producer from a physical IMU sensor instead of the mouse/keys.

## Arduino side

1. Install the Adafruit BNO055 sensor library in the Arduino IDE:
   <https://github.com/adafruit/Adafruit_BNO055>
2. Wire the Arduino to an Adafruit BNO055 breakout board.
3. Connect the Arduino to your computer over USB.
4. On Linux, allow serial access so the app and the IDE can talk to it:

   ```sh
   sudo chmod a+rw /dev/ttyACM0
   ```

5. Open `hardware-code/bno055_abs_orientation/bno055_abs_orientation.ino`
   in the Arduino IDE and upload it to the board.

## 3D Audio Producer side

1. Start the app, go to **Listener → Setup Serial**.
2. On Linux, type `/dev/ttyACM0` into the serial-port field and click
   **Setup**, then **Ok**.
3. Go to **Listener → Edit Listener**.
4. Check the **External Device Orientation** box.
5. The listener orientation is now driven by the physical BNO055 IMU while
   audio plays through the sound producer track.