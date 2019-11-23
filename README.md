# Arduino-Jetson-nano-stepper-interface
Control Arduino uno/mega/nano with Jetson nano linux command line


## Arduino installation
  - Follow this repo to install Arduino on Jetson nano: https://github.com/JetsonHacksNano/installArduinoIDE.git
  - To get Temporary access to USB: `sudo chown <user-name> /dev/tty<usb>` and `sudo chmod a+rw /dev/tty<usb>`
  - To set Permenantly change USB device permission: http://ask.xmodulo.com/change-usb-device-permission-linux.html
  - To control arduino from Command line Source:https://github.com/arduino/arduino-cli
  
# Hardware requirement

| Specification | Value |
| ------------- | ------------- |
| Arduino | Mega 2560 |
| 3 Stepper with 6A driver  |87kgcm torque |
| 3 limit switch | 5v |
| UART | USB-UART cable |
