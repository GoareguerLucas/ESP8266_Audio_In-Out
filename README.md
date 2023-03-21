# ESP8266_Audio_In-Out

This code is write for ESP8266, it's tested on NodeMCU ESP-12E.<br>
Is take audio from ADC input (A0) and write it to I2S output (Rx).<br>
You need to switch your cpu frequency to 160Mhz (Arduino IDE : Tools -> CPU Freqency).<br>

I suggest to use this schema for circuit but it's works but it's upgradable :

![Schema](schema.jpg "Schema")


Sources :
- ![Input](https://arduino.stackexchange.com/questions/48640/esp8266-system-adc-read-fast-always-give-1024-as-output)
- ![Output](https://www.hackster.io/janost/audio-hacking-on-the-esp8266-fa9464#toc-rearrange-the-code-for-isr-1)
