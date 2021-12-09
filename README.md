# thermo L1
Repository for L1 thermal experiments Season 3    
   
**#[User mode]**

1/Install arduino  
2/if on windows or mac install driver for CP2104 on linux nothing else to do    
2/Open serial port on arduino with speed (115200)  
3/Push 1 time on start button to strat the movuino   


**##basic commands**

**?**  : ping reply a ping  
**l**: live mode => give you live data  
**L**: stop live mode  
**r or 1 push on button** :  recording mode recodr on flash memory data  
**R or second push to button** : stop recording  
**p** : get data from memory  
**P** : clear data  

NB : each recording erase the previous   
NB2 : the format of data can be customized in the firmware in expert mode   

**#[Expert Mode] if you need to change the firmware**   
 
1/(if on nmac on windows) Install CP2104 Drivers  https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

2/The support for ESP32 has to be installed following those instructions (only code 1.0.6 is tested working) : 
https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md

3/Restart arduino 

4/ The board to use in arduino is ESP32-dev

5/ install libraries   
    - Adafruit_Sensor (latest)  
    - Adafruit_BME280 (latest)  
    - Adafruit_NeoPixel (latest)  
