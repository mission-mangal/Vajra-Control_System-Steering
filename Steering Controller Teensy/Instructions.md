# Instructions for using this code base on microcontroller- Teensy 4.1.
## Tools used/preferred
*   VS Code
    *   Standard and widely used IDE
*   Github
    *   For maintaining the code and Version Control
*   PlatformIO
    *   Cross Platform Development and Testing of embedded systems.

## Instruction for first time users.
*   setup the udev rule in your local machine(if using linux), so that you can use teensy.
    ```bash
    cd /etc/udev/rules.d/
    sudo touch 00-teensy.rules
    sudo nano 00-teensy.rules
    ```
*   paste the below text into this file and save it.
```txt

    ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", ENV{ID_MM_DEVICE_IGNORE}="1", ENV{ID_MM_PORT_IGNORE}="1"
    ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04[789a]*", ENV{MTP_NO_PROBE}="1"
    KERNEL=="ttyACM*", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", MODE:="0666", RUN:="/bin/stty -F /dev/%k raw -echo"
    KERNEL=="hidraw*", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", MODE:="0666"
    SUBSYSTEMS=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="04*", MODE:="0666"
    KERNEL=="hidraw*", ATTRS{idVendor}=="1fc9", ATTRS{idProduct}=="013*", MODE:="0666"
    SUBSYSTEMS=="usb", ATTRS{idVendor}=="1fc9", ATTRS{idProduct}=="013*", MODE:="0666"

```
*   After this reboot the system.

##  Instructions afte the initial setup is completed

*   First connect the teensy board to your local computer, where you have the access to the repository, make sure to use a data cable not just a power cable.
*   Check whether the board has been successfuly powered on or not?
*   If yes, then press the reset button on the board, so that you can upload new bootloader file.
*   Now build the project and upload it on to the board
*   Enjoyy your life

For getting a deep dive into Teensy or for any query please refer to the resource.
##  [Teensy PJRC](https://www.pjrc.com/teensy/tutorial.html)