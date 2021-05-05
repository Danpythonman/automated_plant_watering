Introduction
============

This is the code for an automated plant-watering system to run on an Arduino-compatible computer board.
It detects the moisture level from a moisture sensor in the soil.
If the soil is too dry the pump (which is submerged in water) is activated to water the plant.
This was originally a project for univserity and involved a Java program on a second computer to communicate with the Arduino-compatible board, but it has since been refactored to use only C++, running only on the board without a second computer.


Requirements
============

In terms of software, this module requires two other modules:

* ```Arduino.h```
* ```U8x8lib.h```

In terms of hardware, this module has only been used on a [Grove Beginner Kit for Arduino](https://wiki.seeedstudio.com/Grove-Beginner-Kit-For-Arduino/) with the modification that the rotary potentiometer was removed from the bord.
Presumably, this module should work on any Arduino-compatible board, provided it has all the necesseary components, but the code would have to be changed to use the correct pins.
The components used by this module are:

* Arduino-compatible board
* Moisture sensor
* MOSFET switch
* Pump and water tube
* Battery pack
* USB to Micro-USB cable
* Button
* LED
* OLED display

Explanation of Components
-------------------------

The USB to Micro-USB cable connects the computer to the board to upload the module to the board, and it connects the board to a power source to power the board. The moisture sensor measures the moisture level of the soil, the MOSFET switch turns the pump on/off and the battery pack connects to the MOSFET switch and powers the pump.
The pump is submerged in water and, when activated, pumps water through the tube into the plant.
The LED turns on only when the pump is active.
The OLED display turns on only when the button is pressed down and displays the current moisture level of the soil and the time until the moisture level will be evalualted for watering (which happens every 10 minutes).
Moisture levels above 600 means the soil is dry, otherwise the soil is wet.


Installation
============

Download this file, compile it, and upload it to the Arduino-compatible board.


Configuaration
==============

This module has no menu or modifiable settings.


Contact
=======

Daniel Di Giovanni - <dannyjdigio@gmail.com>
