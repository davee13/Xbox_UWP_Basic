#pragma once
#include "pch.h"
#include "InputDevices.h"





std::vector<RawGameController^> myRawGameControllers;
std::vector<gameControllerData*> gControllerData;  //this is the main camera that will be used throughout the game

string controllerDebugText = "";
string newLine = "\n";

//default to -1 keyboard and mouse - no controllers on start up
int selectedController = -1;
int numControllers = 0;

gameControllerData::~gameControllerData()
{
}


//default constructor
gameControllerData::gameControllerData()
{
	upKeyPressed = false;
	downKeyPressed = false;
	leftKeyPressed = false;
	rightKeyPressed = false;

	leftStickX = 0.0;
	leftStickY = 0.0;
	rightStickX = 0.0;
	rightStickY = 0.0;
	leftTrigger = 0.0;
	rightTrigger = 0.0;

	hasRunOnce = false;


}

void gameControllerData::Poll(int i)
{
	//loop for each controller
	//for (DWORD i = 0; i < numControllers; i++)    // loop through each world object
	{



		auto btnCount = myRawGameControllers[i]->ButtonCount;
		auto currentAxisCount = myRawGameControllers[i]->AxisCount;
		auto currentSwitchCount = myRawGameControllers[i]->SwitchCount;

		uint16_t  HWPid = myRawGameControllers[i]->HardwareProductId;
		uint16_t  HWVendor = myRawGameControllers[i]->HardwareVendorId;
		Platform::String^ name = myRawGameControllers[i]->DisplayName->ToString();

		string cName = PlatformString2string(name);

		gControllerData[i]->ButtonCount = btnCount;
		gControllerData[i]->AxisCount = currentAxisCount;
		gControllerData[i]->SwitchCount = currentSwitchCount;

		gControllerData[i]->HWPid = HWPid;
		gControllerData[i]->HWVendor = HWVendor;
		gControllerData[i]->name = name;
		gControllerData[i]->cName = cName;


		//controllerDebugText += "Controller " + to_string(i) + ": " + cName;
		//controllerDebugText += newLine + "Button Count: " + to_string(btnCount);
		//controllerDebugText += newLine + "HardWare ID: " + to_string(HWVendor);

		//controllerDebugText += newLine;

		auto currentButtonReading = ref new Platform::Array<bool>(btnCount);
		auto currentSwitchReading = ref new Platform::Array<GameControllerSwitchPosition>(currentSwitchCount);
		auto currentAxisReading = ref new Platform::Array<double>(currentAxisCount);



		myRawGameControllers[i]->GetCurrentReading(
			currentButtonReading,
			currentSwitchReading,
			currentAxisReading);

		////copy data into the vector arrays
		//for (DWORD j = 0; j < gControllerData[i]->ButtonCount; j++)    // loop through each world object
		//{
		//	gControllerData[i]->ButtonReading.push_back(currentButtonReading[j]);
		//}
		//for (DWORD j = 0; j < gControllerData[i]->SwitchCount; j++)    // loop through each world object
		//{
		//	gControllerData[i]->SwitchReading.push_back(currentSwitchReading[j]);
		//}
		//for (DWORD j = 0; j < gControllerData[i]->AxisCount; j++)    // loop through each world object
		//{
		//	gControllerData[i]->AxisReading.push_back(currentAxisReading[j]);
		//}




		//check Axis state
		if (currentAxisCount == 6)
		{
			//Xbox controllers have 6 axis: 2 for each stick and one for each trigger

			//leftStickX = 0;
			//xbox 360 controllers
			if (HWVendor == 1118)
			{
				//leftStickX = 0;
					//normalize in range of -1.0 to 1.0 - 0 = center of stick
				leftStickX = (currentAxisReading[0] - 0.5) * 2.0;
				leftStickY = (currentAxisReading[1] - 0.5) * 2.0;
				rightStickX = (currentAxisReading[2] - 0.5) * 2.0;
				rightStickY = (currentAxisReading[3] - 0.5) * 2.0;
				leftTrigger = (currentAxisReading[4] - 0.5) * 2.0;
				rightTrigger = (currentAxisReading[5] - 0.5) * 2.0;

				//check deadzone
				if (abs(leftStickX) < 0.2) { leftStickX = 0; hasRunOnce = true; }
				if (abs(leftStickY) < 0.2) { leftStickY = 0; hasRunOnce = true; }
				if (abs(rightStickX) < 0.2) { rightStickX = 0; hasRunOnce = true; }
				if (abs(rightStickY) < 0.2) { rightStickY = 0; hasRunOnce = true; }

				//prevent zero on first reading
				if (hasRunOnce == false) {
					if (currentAxisReading[0] == 0) { leftStickX = 0.0; }
					if (currentAxisReading[1] == 0) { leftStickY = 0.0; }
					if (currentAxisReading[2] == 0) { rightStickX = 0.0; }
					if (currentAxisReading[3] == 0) { rightStickY = 0.0; }

				}

				//controllerDebugText += "Left Stick X :" + to_string(leftStickX);
				//controllerDebugText += "Right Stick X :" + to_string(rightStickX);
			}
			//ps4 controller
			if (HWVendor == 1356)
			{

				//normalize in range of -1.0 to 1.0 - 0 = center of stick
				leftStickX = currentAxisReading[1];
				leftStickY = currentAxisReading[0];
				rightStickX = currentAxisReading[5];
				rightStickY = currentAxisReading[2];
				rightTrigger = currentAxisReading[4];
				leftTrigger = currentAxisReading[3];

				leftStickX = ((1 - leftStickX) - 0.5) * 2.0;
				leftStickY = ((leftStickY)-0.5) * 2.0;
				rightStickX = ((1 - rightStickX) - 0.5) * 2.0;
				rightStickY = ((rightStickY)-0.5) * 2.0;
				//leftTrigger = ((1 - leftTrigger) - 0.5) * 2.0;
				//rightTrigger = ((1 - rightTrigger) - 0.5) * 2.0;
				//controllerDebugText += "Left Stick X :" + to_string(leftStickX);

			}

		}



		//check switch state
		for (uint32_t j = 0; j < currentSwitchCount; j++)
		{
			auto swithPosition = currentSwitchReading[j];

			if (swithPosition == GameControllerSwitchPosition::Up)
			{
				//controllerDebugText += "Up is pressed.";
				upKeyPressed = true;
			}
			if (swithPosition == GameControllerSwitchPosition::Down)
			{
				//controllerDebugText += "Down is pressed.";
				downKeyPressed = true;
			}
			if (swithPosition == GameControllerSwitchPosition::Left)
			{
				//controllerDebugText += "Left is pressed.";
				leftKeyPressed = true;
			}
			if (swithPosition == GameControllerSwitchPosition::Right)
			{
				//controllerDebugText += "Right is pressed.";
				rightKeyPressed = true;
			}
			//if (GameControllerSwitchPosition::Down ==
			//	(currentSwitchReading[j] & GameControllerSwitchPosition::Down))
			//{
			//	// The switch is in the up position.
			//	controllerDebugText += "Down is pressed.";
			//}
			//myRawGameControllers[i]->
			//if (currentButtonReading[j])
			//{

			//}
		}


		//check each button state
		for (uint32_t j = 0; j < btnCount; j++)
		{
			if (currentButtonReading[j])
			{
				GameControllerButtonLabel buttonLabel = myRawGameControllers[i]->GetButtonLabel(j);

				//xbox 360 controllers
				if (HWVendor == 1118) {
					if (buttonLabel == GameControllerButtonLabel::XboxA)
					{
						controllerDebugText += "A is pressed.";
					}
					if (buttonLabel == GameControllerButtonLabel::XboxB)
					{
						controllerDebugText += "B is pressed.";
					}
					if (buttonLabel == GameControllerButtonLabel::XboxX)
					{
						controllerDebugText += "X is pressed.";
					}
					if (buttonLabel == GameControllerButtonLabel::XboxY)
					{
						controllerDebugText += "Y is pressed.";
					}
					if (buttonLabel == GameControllerButtonLabel::XboxLeftBumper)
					{
						controllerDebugText += "LB is pressed.";
					}
					if (buttonLabel == GameControllerButtonLabel::XboxRightBumper)
					{
						controllerDebugText += "RB is pressed.";
					}
					if (buttonLabel == GameControllerButtonLabel::XboxUp)
					{
						controllerDebugText += "Up is pressed.";
						upKeyPressed = true;
					}
					if (buttonLabel == GameControllerButtonLabel::XboxDown)
					{
						controllerDebugText += "Down is pressed.";
						downKeyPressed = true;
					}
					if (buttonLabel == GameControllerButtonLabel::XboxLeft)
					{
						controllerDebugText += "Left is pressed.";
						leftKeyPressed = true;
					}
					if (buttonLabel == GameControllerButtonLabel::XboxRight)
					{
						controllerDebugText += "Right is pressed.";
						rightKeyPressed = true;
					}


				}


				//sony ps4 controller
				if (HWVendor == 1356) {
					if (currentButtonReading[1])
					{
						controllerDebugText += "X is pressed.";
					}
					if (currentButtonReading[2])
					{
						controllerDebugText += "O is pressed.";
					}
					if (currentButtonReading[0])
					{
						controllerDebugText += "Square is pressed.";
					}
					if (currentButtonReading[3])
					{
						controllerDebugText += "Triangle is pressed.";
					}
					if (currentButtonReading[4])
					{
						controllerDebugText += "LB is pressed.";
					}
					if (currentButtonReading[5])
					{
						controllerDebugText += "RB is pressed.";
					}
					if (currentButtonReading[10])
					{
						controllerDebugText += "Up is pressed.";
						upKeyPressed = true;
					}
					if (currentButtonReading[12])
					{
						controllerDebugText += "Down is pressed.";
						downKeyPressed = true;
					}
					if (currentButtonReading[11])
					{
						controllerDebugText += "Left is pressed.";
						leftKeyPressed = true;
					}
					if (currentButtonReading[13])
					{
						controllerDebugText += "Right is pressed.";
						rightKeyPressed = true;
					}
				}


			}
		}
	}
}