#include "pch.h"
#include "App.h"
#include <ppltasks.h>
#include <ppl.h>
#include <algorithm>

using namespace Windows::Devices::Input;

using namespace App1;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

//#include <concrt.h>
//#include <winrt/Windows.Gaming.Input.h>
//
//using namespace Windows::Gaming::Input;
//using namespace Windows::Gaming;
//
//
//
#include <collection.h>
using namespace Platform::Collections;
//
//using namespace Platform;
//using namespace Windows::Foundation;
using namespace Windows::Gaming::Input;
//using namespace concurrency;

//CoreWindow^ iWindow;

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new App();
}

App::App() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

// The first method called when the IFrameworkView is being created.
void App::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &App::OnResuming);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_shared<DX::DeviceResources>();
}

// Called when the CoreWindow object is created (or re-created).
void App::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDisplayContentsInvalidated);

	//input devices here

	// register handler for relative mouse movement events
	Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved +=
		ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &App::OnMouseMoved);

	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyDown);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyUp);

	// Prevent the XBOX controller B button from automatically closing the game
	// register handler for relative back button pressed - override default 'B' button behavior for xbox
	Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->BackRequested +=
		ref new EventHandler<BackRequestedEventArgs^>(this, &App::BackRequested);

	// Register gamepad added and removed events.
	RawGameController::RawGameControllerAdded += ref new EventHandler<RawGameController^>(this, &App::OnRawControllerAdded);
	RawGameController::RawGameControllerRemoved += ref new EventHandler<RawGameController^>(this, &App::OnRawControllerRemoved);

	m_deviceResources->SetWindow(window);

	window->Activate();
	window->SetPointerCapture();
	window->PointerCursor = nullptr; //hide the pointer - we are not clicking on windows or buttons


}



// index of an element
int getIndex(vector<RawGameController^> v, RawGameController^ K)
{
	auto it = find(v.begin(), v.end(), K);

	// If element was found
	if (it != v.end())
	{

		// calculating the index
		// of K
		int index = it - v.begin();
		return index;
	}
	else {
		// If the element is not
		// present in the vector
		return -1;
	}
}



//add controller
void App::OnRawControllerAdded(Platform::Object^ sender, RawGameController^ args)
{
	concurrency::critical_section myLock{};

		// Test whether the raw game controller is already in myRawGameControllers; if it isn't, add it.
		concurrency::critical_section::scoped_lock lock{ myLock };
		auto it{ std::find(begin(myRawGameControllers), end(myRawGameControllers), args) };

		if (it == end(myRawGameControllers))
		{
			// This code assumes that you're interested in all raw game controllers.
			myRawGameControllers.push_back(args);
			//gameControllerData* gameControllerData2 = new gameControllerData;
			//gameControllerData2.btnCount = 0;
			gControllerData.push_back(new gameControllerData);
		}
}

//remove controller
void App::OnRawControllerRemoved(Platform::Object^ sender, RawGameController^ args)
{
	concurrency::critical_section myLock{};
	// Test whether the raw game controller is already in myRawGameControllers; if it isn't, add it.
	concurrency::critical_section::scoped_lock lock{ myLock };
	//get index 
	int index = getIndex(myRawGameControllers, args);
	if (index != -1) {
		//remove controller from controller arrays
		myRawGameControllers.erase(myRawGameControllers.begin() + index);
		gControllerData.erase(gControllerData.begin() + index);
	}
	
	//step down to the next available controller
	selectedController = selectedController - 1;
	if (selectedController <= -1) { selectedController = -1; }

}



// Initializes scene resources, or loads a previously saved app state.
void App::Load(Platform::String^ entryPoint)
{
	if (m_main == nullptr)
	{
		m_main = std::unique_ptr<App1Main>(new App1Main(m_deviceResources));
	}
}

// This method is called after the window becomes active.
void App::Run()
{
	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			//update any non-rendering code
			m_main->Update();
			
			//update the camera when all inputs have been processed
			Camera1->UpdateCamera();



			if (m_main->Render())
			{
				m_deviceResources->Present();
			}
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
			
		}
	}
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void App::Uninitialize()
{
}

// Application lifecycle event handlers.

void App::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
        m_deviceResources->Trim();

		// Insert your code here.

		deferral->Complete();
	});
}

void App::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// Insert your code here.
}

// Window event handlers.

void App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_deviceResources->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	m_main->CreateWindowSizeDependentResources();
}

void App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void App::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation event handlers.

void App::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void App::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void App::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	m_deviceResources->ValidateDevice();
}


void App::OnMouseMoved(_In_ Windows::Devices::Input::MouseDevice^ mouseDevice, _In_ Windows::Devices::Input::MouseEventArgs^ args)
{
	//flag that the mouse moved
	iMouse->mouseMoved = true;

	//struct float2 {
	//	float x;
	//	float y;
	//};

	float rGain = 0.005;
	float2 ROTATION_GAIN{ rGain, rGain };

	float2 pointerDelta;
	pointerDelta.x = static_cast<float>(args->MouseDelta.X);
	pointerDelta.y = static_cast<float>(args->MouseDelta.Y);

	iMouse->rotationDeltaX = -pointerDelta.x * ROTATION_GAIN.x;   // scale for control sensitivity
	iMouse->rotationDeltaY = -pointerDelta.y * ROTATION_GAIN.y;   // scale for control sensitivity

	//// limit pitch to straight up or straight down
	//float limit = (float)(M_PI / 2) - 0.01f;
	//m_pitch = (float)__max(-limit, m_pitch);
	//m_pitch = (float)__min(+limit, m_pitch);

	//// keep longitude in useful range by wrapping
	//if (m_yaw > M_PI)
	//	m_yaw -= (float)M_PI * 2;
	//else if (m_yaw < -M_PI)
	//	m_yaw += (float)M_PI * 2;

		//Rotate Camera
	//NewCamera.SetPitch(-DInput->MouseState.lY * 0.01f);
	//NewCamera.SetYaw(-DInput->MouseState.lX * 0.01f);

	//Camera1->SetPitch(m_pitch);
	//Camera1->SetYaw(m_yaw);


}


//the default xbox one behavior is to process as a back button click.....just bypass that 
void App1::App::BackRequested(Platform::Object^ sender, BackRequestedEventArgs^ e)
{
	e->Handled = true;
}

//handle keyboard event down
void App1::App::OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{

	//exit the app
	if (args->VirtualKey == Windows::System::VirtualKey::Escape) {
		exit(-1);
	}

	//tab key pressed
	if (args->VirtualKey == Windows::System::VirtualKey::Tab) {
		selectedController++;
		//loop back to keyboard and mouse
		if (selectedController > numControllers-1) {
			selectedController = -1;
		}
		
		iKeyboard->tab = true;
		
	}

	//control / shift / alt keys pressed
	if (args->VirtualKey == Windows::System::VirtualKey::LeftControl) {
		iKeyboard->lCtrl = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::RightControl) {
		iKeyboard->rCtrl = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::LeftShift) {
		iKeyboard->lShift = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::RightShift) {
		iKeyboard->rShift = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Space) {
		iKeyboard->space = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Enter) {
		iKeyboard->enter = true;
	}

	//WASD movement
	if (args->VirtualKey == Windows::System::VirtualKey::W || args->VirtualKey == Windows::System::VirtualKey::Up) {
		iKeyboard->up = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::S || args->VirtualKey == Windows::System::VirtualKey::Down) {
		iKeyboard->down = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::A || args->VirtualKey == Windows::System::VirtualKey::Left) {
		iKeyboard->left = true;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::D || args->VirtualKey == Windows::System::VirtualKey::Right) {
		iKeyboard->right = true;
	}

};

//handle keyboard event up
void App1::App::OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{


	//if (args->VirtualKey == Windows::System::VirtualKey::Escape) {
	//	exit(-1);
	//}

	if (args->VirtualKey == Windows::System::VirtualKey::Tab) {
		iKeyboard->tab = false;
	}

	//control / shift / alt keys pressed
	if (args->VirtualKey == Windows::System::VirtualKey::LeftControl) {
		iKeyboard->lCtrl = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::RightControl) {
		iKeyboard->rCtrl = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::LeftShift) {
		iKeyboard->lShift = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::RightShift) {
		iKeyboard->rShift = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Space) {
		iKeyboard->space = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::Enter) {
		iKeyboard->enter = false;
	}

	//WASD movement
	if (args->VirtualKey == Windows::System::VirtualKey::W || args->VirtualKey == Windows::System::VirtualKey::Up) {
		iKeyboard->up = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::S || args->VirtualKey == Windows::System::VirtualKey::Down) {
		iKeyboard->down = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::A || args->VirtualKey == Windows::System::VirtualKey::Left) {
		iKeyboard->left = false;
	}
	if (args->VirtualKey == Windows::System::VirtualKey::D || args->VirtualKey == Windows::System::VirtualKey::Right) {
		iKeyboard->right = false;
	}




};