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
| Jetson Nano | nano dev kit |
| Arduino | Mega 2560 |
| 3 Stepper with 6A driver  |87kgcm torque, without Encoder |
| 3 limit switch | 40-50mm length |
| UART | USB-TTL cable |

# Description

To control steper motor from jetson nano via arduino

  1. At initial, arduino will wait for serial command 'X' from jetson nano (communication over UART)
  2. After receiving 'X' from jetson nano, stepper motor will start rotate with contant speed untill it receives interrupt from 'limit switch' on arduino interrupt pin.
  3. After interrupt, motor will stop and that position will be 0 degree (initial position of motor)
  `Note: Above steps are used here for getting all stepper motors align to initial position `
  4. To rotate stepper motor with precise movement from jetson nano, send command `'G<pos1>A<pos2>B<pos3>C'` to arduino
  `Note: pos1 = theta1 * (motorMicrostep/360) #theta1 in degree`


# Reference 
Stepper Motor control Library for Arduino: https://www.airspayce.com/mikem/arduino/AccelStepper/annotated.html
