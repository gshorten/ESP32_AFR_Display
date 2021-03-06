# ESP32_AFR_Display
 AFR (Air Fuel Ratio) display using TTGO ESP32 module with TFT display.
 This connects to a Speeduino v0.4.c board via Serial3 on the arduino and periodically gets AFR data.
 It then displays the actual AFR on the tft and shows a needle gauge giving visual indication 
 of how rich or lean actual is relative to the target.
 
 Using the bottom button on the ESP32 the display can be switched to show main loops per second or EGO correction +_ percent.
 
 This uses the SpeedData library ( https://github.com/gshorten/SpeedData) to get get the data from the Speeduino secondary interface.   For details on the Speeduino secondary interface: https://wiki.speeduino.com/en/Secondary_Serial_IO_interface.  The SpeedData library simplifies getting the data from the speeduino, there are methods to get actual afr, target afr, EGO, etc..
 
 The TTGO ESP32 module (http://www.lilygo.cn/prod_view.aspx?TypeId=50033&Id=1126&FId=t3:50033:3 )
 has an integrated TFT display, two buttons, integrated bluetooth and wifi, which makes it ideal for this application.  
 
 The code is setup for OTA updates via WiFi, but the initial install has to be done via USB.
 
 The ESP32 is hardwired to the Serial3 port on the Speeduino / Arduino, I am not  using bluetooth as I use that to connect the Speeduino to tunerstudio on my laptop.
 
 The case is from my local electronics store, there are many similar on amazon / ebay.  I am not using the back of the housing, just the clear front.  The back is a sheet of ABS with the threaded inserts from the original back case pushed in.  This is so that the whole package is slimmer.
 
 As the Arduino is 5v and the ESP32 is 3.3 volts there is a level shifter added to a small pcb which also holds the ESP32.
 
 The code is self explanatory but email me if you have questions: geoff.shorten@shortens.ca.
 
 I'm not a professional coder so apologies for the hacky code. Basically I learned c++ to do this project.   But, it works :-)
 
 See my other ESP32 based project, a portable Sonos music system controller: https://github.com/gshorten/ESP32-Sonos-Controller/blob/master/README.md
 Currently documentation for this is sparse, I will update when I get time :-)
