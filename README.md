# ESPCiarko

Some Ciarko exhaust hood came with Bluetooth control. There is app on Android and was app on iOS, but it is not so convinient to use it. After some short investigation I have found out how to control hood from any bluetooth device. This is implementation of it for ESP32. I have also added AEG HOB2HOOD implementation, so hob can control hood. You need only add IR receiver (I used TSOP2236).

## How to build and upload?

Thanks to PlatformIO it is very easy. To build project you should first create `config.h` file. I have attached sample file. Most important part is to provide device address. When everything is ready just type in terminal:

```
pio run -t build
```

Then prepare your ESP32 device and enter programming mode. On standard ESP32 dev board you just hold boot button during powering it up. When device is ready just run this command:

```
pio run -t upload
```

## TODO

* Enable OTA
* Simple HTTP interface
* Optional MQTT
