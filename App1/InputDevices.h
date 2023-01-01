#pragma once
#ifndef INPUTDEVICES_H
#define INPUTDEVICES_H


#include <pch.h>
#include <vector>

#include <collection.h>
#include <string>

#include "Utilities.h"

using namespace Windows::Gaming::Input;
using namespace concurrency;
using namespace Platform::Collections;

using namespace DirectX;
using namespace std;

extern std::vector<RawGameController^> myRawGameControllers;

extern string controllerDebugText;
extern string newLine;

//classes
extern class gameControllerData
{
public:
	gameControllerData();
	~gameControllerData();

	int ButtonCount;
	int AxisCount;
	int SwitchCount;

	uint16_t  HWPid;
	uint16_t  HWVendor;
	Platform::String^ name;
	string cName = PlatformString2string(name);

	//directional pad
	bool upKeyPressed;
	bool downKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;

	//thumb sticks
	double leftStickX;
	double leftStickY;
	double rightStickX;
	double rightStickY;
	double leftTrigger;
	double rightTrigger;

	//query device and get data
	void Poll(int i);

private:
	bool hasRunOnce;

};//end game controller class



extern std::vector<gameControllerData*> gControllerData;
extern int selectedController;
extern int numControllers;




#endif