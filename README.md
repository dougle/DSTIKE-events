# DSTIKE-events
A playground/example of event based programming on the DSTIKE ESP8266 Watch V2

### Eventually
[Eventually](https://github.com/johnnyb/Eventually) is a library for adding event listeners to a chain, the chain is periodically iterated and each event is evaluated for any trigger event(s), if a listener is triggered the relevant [action](https://github.com/dougle/DSTIKE-events/blob/main/src/actions.h) is called.

A [custom listener](https://github.com/dougle/DSTIKE-events/blob/main/src/listeners.h) is defined for listening for the ESPs wifi adapter status.

Note that Eventually's [loop iteration macro](https://github.com/johnnyb/Eventually/blob/master/src/Eventually.h#L101) isn't used, this is because the ESP's watchdog needs a slight delay to keep it happy (fed).

I am currently using a fork of eventually, this is while [PR7](https://github.com/johnnyb/Eventually/pull/7) and [PR12](https://github.com/johnnyb/Eventually/pull/12) are open.

#### Build
A PlatformIO ini is supplied you can build, upload and monitor from the project root with the following command.
```bash
$ pio run -t upload && pio device monitor -f esp8266_exception_decoder
```
or specify the device port:
```bash
$ pio run -t upload --upload-port /dev/ttyUSB0 -v && pio device monitor -p /dev/ttyUSB0 -f esp8266_exception_decoder
```

#### Events
The currently enabled listeners can be found [here](https://github.com/dougle/DSTIKE-events/blob/main/src/main.cpp#L64):
* Three button (pullup) listeners for up, down, and push (select) on the DSTIKE's slide button.  
    Each button will display text on the screen and a display timeout timer will run for ten seconds.
* A timer listener.  
    Each trigger of this one second timer will increment an int and display it in the bottom left corner.
* A WiFi status listener  
    This listener with trigger every time the Wifi's status changes and will display an icon in the top right of the screen.  
    Solid for connected, blinking with an `!` for anything else.  
    If the ssid is not available `WL_NO_SSID_AVAIL`, a timer for ten seconds will start the connection process again. 

#### Serial
Serial is started and contains loads of runtime stuff.

#### WiFi
To setup wifi, create `src/wifi.h` and paste in the following:

```c++
#define WIFI_SSID "your-ssid"
#define WIFI_PASS "9e87b4b63fd7cfb770718150e1b94faf0dac7f1245526c7209746c55aaee1daf"
// or #define WIFI_PASS "your-plain-password"
```

#### Future
Please fork and PR any new event based examples or fixes, all are welcome, i'll be updating this when i need to play around with other events on the DSTIKE.

Raise [issues](https://github.com/dougle/DSTIKE-events/issues) if you find problems or have any questions.