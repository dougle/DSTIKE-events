/**************************************************************************
 * DSTRIKE ESP8266 Watch 2 Button Events
 *
 * https://www.espressif.com/sites/default/files/documentation/Espressif_FAQ_EN.pdf
 * 
 *
 * no warranty is given.
**************************************************************************/

#include <Eventually.h>

#include <OLEDDisplayFonts.h>
#include <SH1106Wire.h>

#include <ESP8266WiFi.h>

#include "images.h"
#include "listeners.h"

// Button pin definitions
#define BUTTON_UP 12
#define BUTTON_DOWN 13
#define BUTTON_SELECT 14

#define OLED_SDA_PIN D1  // pin 5
#define OLED_SCL_PIN D2  // pin 4
#define OLED_ADDR 60  //0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

EvtManager mgr;
SH1106Wire OLED = SH1106Wire(OLED_ADDR, OLED_SDA_PIN, OLED_SCL_PIN);

#include "actions.h"

void setup() {
	Serial.begin(115200);
	Serial.flush();
	delay(500);

	wifi_connect();

	Serial.println("Start OLED");
	OLED.init();
	OLED.flipScreenVertically();
	OLED.displayOn();
	OLED.clear();
	OLED.display();

	OLED.setColor(WHITE);
	OLED.fillRect(0, 0, OLED_WIDTH, OLED_HEIGHT);
	OLED.display();

	delay(500);

	OLED.clear();
	OLED.display();

	pinMode(BUTTON_UP, INPUT_PULLUP);
	pinMode(BUTTON_DOWN, INPUT_PULLUP);
	pinMode(BUTTON_SELECT, INPUT_PULLUP);

	Serial.println("Start listeners");
	mgr.resetContext();
	mgr.addListener(new EvtPinListener(BUTTON_UP, 100, LOW, (EvtAction)button_up));
	mgr.addListener(new EvtPinListener(BUTTON_DOWN, 100, LOW, (EvtAction)button_down));
	mgr.addListener(new EvtPinListener(BUTTON_SELECT, 100, LOW, (EvtAction)button_select));

	mgr.addListener(new EvtWifiStateListener((EvtAction)wifi_status_change));

	mgr.addListener(new EvtTimeListener(1000, true, (EvtAction)tick));
}

void loop() {
	mgr.loopIteration();
	delay(10); // specific to the ESP8266 with it's software watchdog
}
