# Hardware 

The hardware consists of a Raspberry Pi model 3 B+ and our extension board.
This part of the repository supplies all the required information about the extension board. 

For a more in-depth description, have a look at the <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/wiki/Hardware" target="_blank">`Wiki`</a>.

## Overview
In this folder:
* <a href="./hardware/OrCAD" target="_blank">`OrCAD`</a>: The complete design files for the schematic and the layout. This relies on some standard footprints that are not included (SMD 0804, 0603, ect.). All special footprints are included. 
* <a href="./hardware/PCBProduction" target="_blank">`PCBProduction`</a>: The Gerber files and BOM used to produce the hardware.
* The latest schematic  as a pdf file.
* <a href="./hardware/hw_revision_history.md" target="_blank">`HW Revision History`</a>

## Description

The main component on the extension board is the u-blox module SARA-G350-02X-01, a GSM/GPRS module. 
Additionally, there is a micro SIM card adapter, an SMA antenna connector to connect a cellular antenna and some small additional components for power supply and connection to the PI.

### Power supply and connections to the Raspberry PI
The Raspberry PI is connected through its 40-pin <a href="https://www.raspberrypi.org/documentation/usage/gpio/README.md" target="_blank">`GPIO header`</a>. 

* 5V of power is supplied by the Raspberry PI board (pins 2 and 4 on the header). 
* For communication, UART (pins 6 and 8 on the header) is used.
* All ground pins are connected.
* GPIO 5 and 6 on the Raspberry PI (pin 19 and 21 on the header) are connected to GPIOs of the u-blox module.

## Software Tools

The design files were produced with OrCAD by Cadence.
The version used is 17.2.




