/*
 * actions.h
 */

#ifndef DSTIKE_EVENTS_ACTIONS_H
#define DSTIKE_EVENTS_ACTIONS_H

unsigned tick_count = 0;
EvtTimeListener *clear_button_text_listener;
EvtTimeListener *blink_wifi_listener;
bool blink_state = false;

#include "wifi.h"

void wifi_connect() {
	WiFi.begin(WIFI_SSID, WIFI_PASS);
}

bool clear_button_text() {
	Serial.printf("Clear button text\n");

	const uint8_t* font = ArialMT_Plain_16;
	int y_offset = (OLED_HEIGHT/2) - (font[1]/2);

	OLED.setColor(BLACK);
	OLED.fillRect(0,y_offset,OLED_WIDTH,font[1]);

	OLED.display();

	return false;
}

bool button_press(const char* button) {
	Serial.printf("Button Press %s\n", button);
	const uint8_t* font = ArialMT_Plain_16;

	if(clear_button_text_listener)
		mgr.removeListener(clear_button_text_listener);

	int y_offset = (OLED_HEIGHT/2) - (font[1]/2);

	OLED.setFont(font);
	OLED.setTextAlignment(TEXT_ALIGN_CENTER);

	clear_button_text();

	OLED.setColor(WHITE);
	OLED.drawString(OLED_WIDTH/2, y_offset, String(button));

	OLED.display();

	clear_button_text_listener = new EvtTimeListener(10000, false, (EvtAction)clear_button_text);
	mgr.addListener(clear_button_text_listener);

	return false;
}

bool button_up() { return button_press("up"); }
bool button_down() { return button_press("down"); }
bool button_select() { return button_press("select"); }

void draw_wifi_symbol(EvtWifiStateListener lstn){
	const uint8_t* font = ArialMT_Plain_10;

	OLED.setColor(WHITE);
	OLED.drawXbm(OLED_WIDTH - wifi_01_width, 2, wifi_01_width, wifi_01_height, wifi_01_bits);

	if (lstn.currentState != WL_CONNECTED) {
		OLED.setFont(font);
		OLED.setTextAlignment(TEXT_ALIGN_RIGHT);

		OLED.drawString(OLED_WIDTH - wifi_01_width - 1,0, "!");
	}
}

void clear_wifi_symbol() {
	const uint8_t* font = ArialMT_Plain_10;

	OLED.setColor(BLACK);
	OLED.fillRect(OLED_WIDTH-20,0,20,font[1]);
}

void blink_wifi(EvtWifiStateListener lstn){
	if(blink_state) {
		clear_wifi_symbol();
	} else {
		draw_wifi_symbol(lstn);
	}

	blink_state = !blink_state;

	OLED.display();
}

bool wifi_status_change(EvtWifiStateListener lstn, EvtContext ctx) {
	Serial.printf("WiFi status changed from %s to %s\n", lstn.getStatusName(lstn.previousState), lstn.getStatusName(lstn.currentState));

	clear_wifi_symbol();

	draw_wifi_symbol(lstn);

	if (lstn.currentState != WL_CONNECTED) {
		Serial.printf("WiFi not connected: %d vs %d\n", lstn.currentState, WL_CONNECTED);
		blink_wifi_listener = new EvtTimeListener(1000, true, (EvtAction)blink_wifi);
		mgr.addListener(blink_wifi_listener);
	} else {
		if(blink_wifi_listener)
			mgr.removeListener(blink_wifi_listener);
	}

	OLED.display();

	switch (lstn.currentState) {
		case WL_IDLE_STATUS:
			wifi_connect();
			break;
		case WL_CONNECTION_LOST:
		case WL_DISCONNECTED:
			wifi_connect();
			break;
		case WL_NO_SSID_AVAIL:
			mgr.addListener(new EvtTimeListener(10000, false, (EvtAction)wifi_connect));
			break;

	}

	return false;
}

bool tick() {
//	Serial.println("Tick");

	const uint8_t* font = ArialMT_Plain_10;
	int y_offset = OLED_HEIGHT - font[1];

	OLED.setFont(font);
	OLED.setTextAlignment(TEXT_ALIGN_LEFT);

	OLED.setColor(BLACK);
	OLED.fillRect(0,y_offset,OLED_WIDTH, font[1]);

	OLED.setColor(WHITE);
	OLED.drawString(0, y_offset, String(tick_count));

	OLED.display();

	tick_count++;

	return false;
}


#endif //DSTIKE_EVENTS_ACTIONS_H
