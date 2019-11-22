# Arduino-Jetson-nano-interface
Control Arduino uno/mega/nano with Jetson nano linux command line


## Arduino installation
  - Follow this repo to install Arduino on Jetson nano: https://github.com/JetsonHacksNano/installArduinoIDE.git
  - To get Temporary access to USB: `sudo chown <user-name> /dev/tty<usb>` and `sudo chmod a+rw /dev/tty<usb>`
  - To set Permenantly change USB device permission: http://ask.xmodulo.com/change-usb-device-permission-linux.html
  - To control arduino from Command line Source:https://github.com/arduino/arduino-cli
