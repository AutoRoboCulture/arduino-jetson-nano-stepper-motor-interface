# Arduino-Jetson-nano-stepper-interface
Control Multiple Stepper Motor with Arduino uno/mega/nano and Jetson nano


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
| 3 limit switch | 40-50mm length |
| UART | USB-TTL cable |

# Reference 
Stepper Motor control Library for Arduino: https://www.airspayce.com/mikem/arduino/AccelStepper/annotated.html
