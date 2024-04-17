
# Real-Time Orientation Tracking with a Round LCD Interface
![Static Badge](https://img.shields.io/badge/SoC-Nordic_nrf5340-blue) ![Static Badge](https://img.shields.io/badge/Zephyr_RTOS-v3.3.99_ncs1-green) ![Static Badge](https://img.shields.io/badge/nRF_Connect_SDK-v2.4.2-red) ![Static Badge](https://img.shields.io/badge/nRF_Connect_SDK_Toolchain-v2.4.2-orange) ![Static Badge](https://img.shields.io/badge/IDE-VSCode-blue) ![Static Badge](https://img.shields.io/badge/Extension-nRF_Connect-teal)

-------------------------
## Introduction

This application harnesses the BMI270 IMU sensor and the GC9A01 round LCD Display to showcase real-time orientation detection. Utilizing the LVGL library, it effectively visualizes IMU sensor data on the GC9A01 display, offering a dynamic and interactive user interface. This integration not only demonstrates technical capability but also serves as a robust foundation for further development in wearable and IoT devices that require orientation-based functionality.

-------------------------
**TABLE of CONTENTS**
- [Introduction](#introduction)
- [Features](#features)
- [Folder Structure](#folder-structure)
- [Development Environment](#development-environment)
    - [Dev-Environment Setup](#dev-environment-setup)
    - [Typical Build Log](#typical-build-log)
    - [Typical Flash Log](#typical-flash-log)
- [Pin Mapping](#pin-mapping)
- [Orientation Detection System Flow](@orientation-detection-system-flow)
- [Log Output](#log-output)
    - [Typical Program Output Log](#typical-program-output-log)

-------------------------
## Features

- Real-time orientation detection using the BMI270 IMU sensor.
- Visual feedback through the GC9A01 round LCD display.
- Utilization of the LVGL library for graphical user interface creation.
- Development with Nordic nRF5340 DK, supporting advanced features and connectivity.
- Integration with VSCode and nRF Connect SDK for streamlined development and debugging.

-------------------------

## Folder Structure

```Tree

├── app.overlay                                                         # User Defined & Changes for Device tree
├── build                                                         # Build Directory, Should exist after an attempt to build.
├── CMakeLists.txt                                                          # Root level CMakeLists, this is where you should add any more source files so compiler takes it.
├── datasheet                                                         # datasheet for BMI270 IMU sensor and GC9A01 LCD driver
│   ├── BMI270_datasheet.pdf
│   └── GC9A01A_datasheet.pdf
├── dts                                                         # Custom Peripherals Binding Definition in yaml file.(GC9A01..)
│   └── bindings    
│       └──  display
│            └── waveshare,gc9a01.yaml
├── Kconfig
├── misc                                                         # images
│   └──  Hardware_bring-up.png
├── prj.conf                                                         # Default conf file for user selected config unless other files specified in compiler options.
├── README.rst                                                         # Readme file for the project.
├── sample.yaml                                                         # BMI270 Sensor Sample related configuration file.
├── src                                                          # Source Files resides in this folder.
│   ├── gc9a01.c
│   └── main.c
└── ui                  # UI C array
    ├── battery_50_percentage.c
    ├── bluetooth_connected.c
    ├── cairdio_and_rice_logo.c
    └── cairdio_logo.c
```

-------------------------

## Development Environment
### Dev-Environment Setup

Required Tools and Software to build the project:(**CheckList**)<br>
- **nRF Connect Software** : [nRF Connect For Desktop](#https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop/download)
  - **Programmer** Sub-Application.
  - **Serial Terminal** Sub-Application.
  - **Toolchain Manager** Sub-Application: Which manages toolchain and sdk's version.
    - **Install Toolchain**: From *Toolchain Manager* Install the Latest Toolchain.
- Compatible IDE: Segger IDE (Commercial) | **VSCode IDE**.
- A **Must** Extension: With VSCode **nrfConnect Extension** is required.

```
Note: Steps and software tools are also available for Linux & macOS.
```

Any project should have these files with exact names:

```	
prj.conf            [Exact Name]
main.c              [any name but a main entry source file to exist]
CMakeLists.txt      [supply main.c as source to compiler-input]
app.overlay         [exact name: contains user specific configs]

```

#### Workspace Adaption on VSCode IDE & Compilation

- Go to ```NRF Connect Extension(Select)``` from Left Extension selection Vertical Menu Bar.
- **Open As Existing Project** from ```NRF Connect Extension(Select) -> (Under Welcome Panel)Open an Existing Application -> Select the Project Folder.```
- Select A Board(**Add Build Configuration**) from under "Applications" Panel -> ```nrf5340dk_nrf5340_cpuapp``` -> rest of the options are default.
- Enable debug Options if provided by the UI. Latest or Future NRF Connect UI may not have this option.

### Typical Build Log

```
[369/369] Linking C executable zephyr/zephyr.elf
Memory region         Used Size  Region Size  %age Used
           FLASH:      474888 B         1 MB     45.29%
             RAM:       75512 B       448 KB     16.46%
        IDT_LIST:          0 GB         2 KB      0.00%
 *  Terminal will be reused by tasks, press any key to close it. 
```

### Typical Flash Log

```
Flashing build to 1050096938
west flash -d /Users/pisces/Downloads/Nordic_Project/nrf53_bmi270+gc9a01/build --skip-rebuild --dev-id 1050096938 --erase

-- west flash: using runner nrfjprog
-- runners.nrfjprog: mass erase requested
-- runners.nrfjprog: Flashing file: /Users/pisces/Downloads/Nordic_Project/nrf53_bmi270+gc9a01/build/zephyr/zephyr.hex
[ #################### ]   0.405s | Erase file - Done erasing                                                          
[ #################### ]   3.127s | Program file - Done programming                                                    
[ #################### ]   3.196s | Verify file - Done verifying                                                       
Applying pin reset.
-- runners.nrfjprog: Board with serial number 1050096938 flashed successfully.
 *  Terminal will be reused by tasks, press any key to close it. 
```

-------------------------
## Pin Mapping

This section describes the pin mapping of the target hardware for which this version of firmware is build.

| **DESCRIPTION**                           |  **Name - Pin**  | **nRF5340 Port** |
|-------------------------------------------|:----------------:|-----------------:|
| **GC9A01 Round LCD Display Pins**         |                  |                  |
| Serial Clock                              |      SCL/D0      |            P1.15 |
| Master Out Slave In                       |      MOSI/D1     |            P1.13 |
| Chip Select                               |        CS        |            P1.12 |
| Data/Command                              |        DC        |            P1.11 |
| Rest                                      |        RST       |            P1.10 |
| Back Light (didn't use)                   |        BL        |            P1.06 |
| VCC / 3.3V                                |        VCC       |              VDD |
| GND                                       |        GND       |              GND |
| **BMI270 IMU Sensor Pins**                |                  |                  |
| Serial Clock                              |        SCL       |            P0.08 |
| Master Out Slave In                       |     MOSI/SDA     |            P0.09 |
| Master In Slave Out                       |     MOSI/SDO     |            P0.10 |
| Chip Select                               |        CS        |            P0.11 |
| VCC / 3.3V                                |       3.3V       |              VDD |
| GND                                       |       GND        |              GND |
|                                           |                  |                  |
                          
-------------------------
## Orientation Detection System Flow

```
Start
  |
  v
Initialize nRF5340 SoC
  |
  v
Initialize BMI270 IMU Sensor and GC9A01 LCD Display
  |
  v
Display Cairdio & Rice University Logo (3 seconds)
  |
  v
Enter Home Page
  |-> Display Battery Percentage (Top Right)
  |-> Display Bluetooth Icon as Disconnected (Top Left)
  |-> Display Cairdio Logo (Top Center)
  |-> Display Text: "Hold the device for 10 seconds" (Center)
  |
  v
Bluetooth Connection Status Check
  |-- (If Connected) --> Update Bluetooth Icon to Connected
  |-- (If Disconnected) -> Show Disconnected Icon and Return to Home Page
  |
  v
Received Start Recording Command
  |
  v
Orientation Detection
  |-> Correct Placement: "Stay still!" + Green Status Slider Knob
  |-> A Bit Left: "<- Move to right" + Red Status Slider Knob
  |-> A Bit Right: "Move to left ->" + Red Status Slider Knob
  |
  v
Finish Recording (10 Seconds)
  |-> Show UI Message:  "Check your mobile for the result"
  |
  v
End

```

-------------------------

## Log Output

### Typical Program Output Log
- **Select Baud Rate: 115200.**
```
*** Booting Zephyr OS build v3.3.99-ncs1-2 ***
[00:00:00.734,985] <inf> app: Display initialized
[00:00:00.735,046] <inf> app: Sensor initialized, device 0x2c61c name is BMI270
[00:00:00.747,070] <inf> app: IMU sensor configured successfully.
[00:00:00.747,100] <inf> app: Loading logo...
[00:00:03.823,883] <inf> app: Loading menu...
[00:00:07.034,088] <inf> app: Starting orientation detection...
[00:00:07.034,332] <inf> app: accelerometer's Y-axis value: 0
[00:00:07.037,933] <inf> app: Remaining: 10 seconds...
[00:00:07.941,680] <inf> app: accelerometer's Y-axis value: 0
[00:00:07.945,373] <inf> app: Remaining: 9 seconds...
[00:00:09.073,852] <inf> app: accelerometer's Y-axis value: 0
[00:00:09.077,545] <inf> app: Remaining: 8 seconds...
[00:00:10.205,902] <inf> app: accelerometer's Y-axis value: 0
[00:00:10.209,594] <inf> app: Remaining: 7 seconds...
[00:00:11.338,165] <inf> app: accelerometer's Y-axis value: 0
[00:00:11.341,827] <inf> app: Remaining: 6 seconds...
[00:00:12.470,184] <inf> app: accelerometer's Y-axis value: 0
[00:00:12.473,876] <inf> app: Remaining: 5 seconds...
[00:00:13.601,531] <inf> app: accelerometer's Y-axis value: 0
[00:00:13.605,224] <inf> app: Remaining: 4 seconds...
[00:00:14.733,276] <inf> app: accelerometer's Y-axis value: 0
[00:00:14.736,938] <inf> app: Remaining: 3 seconds...
[00:00:15.865,264] <inf> app: accelerometer's Y-axis value: 0
[00:00:15.868,927] <inf> app: Remaining: 2 seconds...
[00:00:16.997,467] <inf> app: accelerometer's Y-axis value: 0
[00:00:17.001,159] <inf> app: Remaining: 1 seconds...
[00:00:18.129,119] <inf> app: accelerometer's Y-axis value: 0
[00:00:18.132,781] <inf> app: Remaining: 0 seconds...
[00:00:18.142,089] <inf> app: Complete recording
```
----

--- END ---