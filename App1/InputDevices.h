#pragma once
#ifndef INPUTDEVICES_H
#define INPUTDEVICES_H


#include <pch.h>
#include <vector>

#include <collection.h>
#include <string>

#include "Utilities.h"
#include "Camera.h"

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
	//control xbox controllers init state 
	bool hasRunOnce;

};//end game controller class



extern std::vector<gameControllerData*> gControllerData;
extern int selectedController;
extern int numControllers;

extern void updateInputDevices();

//mouse data handler
extern class MouseData
{
public:
	MouseData();
	~MouseData();

	float rotationDeltaX;
	float rotationDeltaY;
	bool mouseMoved;

	//buttom click data
	bool lBtnState;
	bool RlBtnState;

}; //end mouse data class


extern MouseData* iMouse;




//keyboard data handler
extern class KeyboardData
{
public:
	KeyboardData();
	~KeyboardData();

	//keys
	bool tab;

	bool up;
	bool down;
	bool left;
	bool right;

	bool A;
	bool B;
	bool W;
	bool S;
	bool D;


}; //end mouse data class


extern KeyboardData* iKeyboard;


#endif