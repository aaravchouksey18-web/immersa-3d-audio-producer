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

1. Start the app. In the **Object Creation / Edit** panel, choose
   **Listener** in the *Object Type* list, then press **Edit**.
2. In the **Edit Listener** dialog, tick the **External Device Orientation**
   box and press **OK**. The flag is persisted with the project
   (`docs/overview-of-systems.md` → project management).

> **Status of the driver in this build.** The serial reading code
> (`listener-external.cpp`, `external-orientation-device-serial.cpp`,
> `SimpleSerial.h`) and the wxWidgets-era "Setup Serial" dialog are still in
> the repo but are **not compiled into the default CMake build** (they are
> the only part that needs Boost). Until they are wired in, the checkbox
> saves/loads the external-orientation flag but the listener orientation is
> not yet driven live by the IMU.