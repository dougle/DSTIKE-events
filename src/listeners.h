/*
 * Listeners.h
 */

#ifndef DSTIKE_EVENTS_LISTENERS_H
#define DSTIKE_EVENTS_LISTENERS_H

#include <Eventually.h>

class EvtPullupPinListener : public EvtPinListener {
public:
	EvtPullupPinListener(int pin, EvtAction trigger):EvtPinListener(pin, trigger) {};
	EvtPullupPinListener(int pin, int debounce, EvtAction trigger):EvtPinListener(pin, debounce, trigger) {};

	virtual void setupListener();
};

class EvtWifiStateListener : public EvtListener {
public:
	EvtWifiStateListener(EvtAction action);

	char* getStatusName(int code);
	int previousState;
	int currentState;
	void setupListener();

	bool isEventTriggered();
};


#endif //DSTIKE_EVENTS_LISTENERS_H
