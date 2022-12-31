#pragma once
#ifndef GLOBALITEMS_H
#define GLOBALITEMS_H
//#define NDEBUG

#include <pch.h>
#include "Camera.h"
#include <collection.h>
#include <string>
using namespace Platform::Collections;
using namespace std;
//
//using namespace Platform;
//using namespace Windows::Foundation;
using namespace Windows::Gaming::Input;
using namespace concurrency;



using namespace DirectX;

extern std::vector<RawGameController^> myRawGameControllers;

extern string controllerDebugText;
extern string newLine;



extern double mAccumulator;


//function prototypes
extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string& s);
extern std::string PlatformString2string(Platform::String^ s);



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

	//std::vector<bool> ButtonReading;
	//std::vector<GameControllerSwitchPosition> SwitchReading;
	//std::vector<double> AxisReading;

	bool upKeyPressed;
	bool downKeyPressed;
	bool leftKeyPressed;
	bool rightKeyPressed;

	double leftStickX;
	double leftStickY;
	double rightStickX;
	double rightStickY;
	double leftTrigger;
	double rightTrigger;




	void Poll(int i);

private:


};//end game controller class



extern std::vector<gameControllerData*> gControllerData;
extern int selectedController;
extern int numControllers;


#endif