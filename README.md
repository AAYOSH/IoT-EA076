# IoT-EA076 Descrition
Project developed at the course EA076 - Laboratory of Embedded systems - FEEC, UNICAMP

This project was developed as a Hello World in the IoT projects, allowing our FRKLM25z board to
communicate with the Esp8266 ESP-01 WiFi Module, and being able to send and receive messages using the MQTT protocol
by either apps or programs.

# Configurations

We programmed two Serial Assynchronous modules - UART, the first one to communicate with the ESP-01 module, and
the second to communicate with the computer keyboard. The codes start the inicialization and communicate with the ESP-01 module are described in the ESP8266 CLIENTE MQTT.pdf, which corresponds to the commands that the firmware installed in the module.

Also, We configurated the internal RGB LEDS of the FRKLM25z board to turn on/off according to the commands received from the ESP-01.

In order to accomplish this, we used the Processor Expert builtin functions to configure the modules that were used in this project, the codes and configurations can be seen in the folders Generated_Code and Documentation respectively.


