
# Arduino Keg Tracker
More information on this project is available at [ryansprojects.tech](ryansprojects.tech).

This is a kegerator tracking program using a Wemos D1 Mini Arduino board, a 128x64 OLED screen, an ADXL345 acccelerometer, and a custom 3D printed tap handle in which all of the components reside in. Links to the hardware are below. 

This was my first "real" Arduino project and has been interated upon several times. The Arduino Cloud connection is necessary, as there are several cloud variables set in order for this to work properly. Constant connection is not required, but is helpful.  Variables that need to be set in the Arduino Cloud are:

```
String kegSizeMessage
int kegRemaining
int lastPour
int pintsRemaining
```



## Hardware
[128x64 OLED (yellow + blue)](https://www.amazon.com/gp/product/B072Q2X2LL/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

[ADXL-345 Accelerometer](https://www.amazon.com/gp/product/B01DLG4OU6/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

[Wemos D1 Mini ESP8266](https://www.amazon.com/gp/product/B081PX9YFV/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) with optional [prototype board](https://www.amazon.com/gp/product/B07L756KQS/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

[Tap handle STL](https://www.thingiverse.com/thing:5820755)
