# Revision History Hardware:
This file refers to the HW revisions as specified in the <a href="https://github.com/itsBelinda/ENG5220-2020-Team13/tags" target="_blank">`tags`</a> 

--------------------------------------------------------------------------------
## V1.3, Date 17.03.2020
### Revision description:
Expanded information in BOM.
Cosmetic changes in layout.

### Major changes:
 - BOM to include the manufacturer and part number for each part.
 - minor cosmetic changes in layout to avoid unconnected shapes.

--------------------------------------------------------------------------------
## V1.2, Date 17.03.2020
### Revision description:
Voltage translator direction settings.

### Major changes:
 - Inverted directions for voltage translator. 

--------------------------------------------------------------------------------
## V1.1, Date 11.03.2020
### Revision description:
Added missing supply voltage connection for 3.3 V.

### Major changes:
 - Missing connection to 3.3 V voltage translator (3.3 V to 1.8 V)

--------------------------------------------------------------------------------
## V1.0, Date 26.02.2020
### Revision description:
Initial revision of the expansion board for the Raspberry PI 3 B 
with a u-blox SARA-G350-02X-01 GSM module.
 - power supplied by 5V from Raspberry PI board (pins 2 and 4 on the header)
 - the u-blox module runs on 3.8 V, supplied directly from the board
 - communication pins on the u-blox module run on 1.8 V, supplied by the module
 - micro-SIM card adapter
 - GSM antenna connected through SMA antenna connector
 - for communication, UART (pins 6 and 8 on the header) is used. 
    - The voltage level on those pins is 3.3 V (Raspberry PI UART)
 - GPIO 5 and 6 on the Raspberry PI (pin 19 and 21 on the header) are connected 
   to GPIOs of the module. 

### Major changes:
none 
