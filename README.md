M2X_CC3000
==========

Example Arduino projects for communicating with AT&amp;T's M2X service using a CC3000.

Getting Started
===============

1. Follow the instructions on [https://github.com/sparkfun/m2x-arduino](https://github.com/sparkfun/m2x-arduino) to sign up for an [M2X](https://m2x.att.com) account and install the [jsonlite](https://github.com/citrusbyte/jsonlite) and [m2x-arduino](https://github.com/sparkfun/m2x-arduino) libbraries.
2. Follow the instructions on [https://github.com/sparkfun/SFE_CC3000_Library](https://github.com/sparkfun/SFE_CC3000_Library) to install SparkFun's CC3000 library.
3. Connect a [CC3000 WiFi Shield](https://www.sparkfun.com/products/12071) to the Arduino.
4. Connect an [HTU21D](https://www.sparkfun.com/products/12064) to the Arduino and CC3000 Shield stack. Note that you will need to level shift the I2C lines, as the HTU21D is 3.3V only.
5. Download this repository.
6. Open up M2X_CC3000_Post.ino in Arduino.
7. Update the values AP_SSID, AP_PASSWORD, AP_SECURITY, FEED_ID, STREAM_NAME, and M2X_KEY to match your WiFi and M2X parameters.

Important
=========

The Fetch and LED examples will cause an Uno to run out of RAM. You need to change SERIAL_BUFFER_SIZE from 64 to 32 in HardwareSerial.cpp, which can be found in: C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino