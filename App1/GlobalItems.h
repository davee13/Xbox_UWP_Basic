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


#endif