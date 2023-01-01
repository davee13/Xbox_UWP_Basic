#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H


#include <pch.h>
#include <string>


//function prototypes
extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string& s);
extern std::string PlatformString2string(Platform::String^ s);



#endif