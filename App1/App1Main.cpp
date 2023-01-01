#include "pch.h"
#include "App1Main.h"
#include "Common\DirectXHelper.h"


//
#include <collection.h>
using namespace Platform::Collections;
//
//using namespace Platform;
//using namespace Windows::Foundation;
using namespace Windows::Gaming::Input;
using namespace concurrency;


using namespace App1;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

//#include <winrt/Windows.Gaming.Input.h>

//using namespace winrt;







//std::vector<RawGameController> myRawGameControllers;
//concurrency::critical_section myLock{};

// Loads and initializes application assets when the application is loaded.
App1Main::App1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
	//init the camera
	Camera1 = new Camera();


	//concurrency::critical_section myLock{};

	//for (auto const& rawGameController : RawGameController::RawGameControllers)
	//{
	//	// Test whether the raw game controller is already in myRawGameControllers; if it isn't, add it.
	//	concurrency::critical_section::scoped_lock lock{ myLock };
	//	auto it{ std::find(begin(myRawGameControllers), end(myRawGameControllers), rawGameController) };

	//	if (it == end(myRawGameControllers))
	//	{
	//		// This code assumes that you're interested in all raw game controllers.
	//		myRawGameControllers.push_back(rawGameController);
	//	}
	//}

	//int numControllers = myRawGameControllers.size();
	//if (numControllers > 0) {
	//	
	//}

	//int bb = 0;

	//concurrency::critical_section myLock{};

	//for (auto const& rawGameController : RawGameController::RawGameControllers)
	//{
	//	// Test whether the raw game controller is already in myRawGameControllers; if it isn't, add it.
	//	concurrency::critical_section::scoped_lock lock{ myLock };
	//	auto it{ std::find(begin(myRawGameControllers), end(myRawGameControllers), rawGameController) };

	//	if (it == end(myRawGameControllers))
	//	{
	//		// This code assumes that you're interested in all raw game controllers.
	//		myRawGameControllers.push_back(rawGameController);
	//	}
	//}

	//std::vector<RawGameController> myRawGameControllers;
	//concurrency::critical_section myLock{};


	////check for raw game controllers
	//for (auto const& rawGameController : RawGameController::RawGameControllers())
	//{
	//	// Test whether the raw game controller is already in myRawGameControllers; if it isn't, add it.
	//	concurrency::critical_section::scoped_lock lock{ myLock };
	//	auto it{ std::find(begin(myRawGameControllers), end(myRawGameControllers), rawGameController) };

	//	if (it == end(myRawGameControllers))
	//	{
	//		// This code assumes that you're interested in all raw game controllers.
	//		myRawGameControllers.push_back(rawGameController);
	//	}
	//}

	hasRunOnce = 0;
	int b = 0;
}





App1Main::~App1Main()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void App1Main::CreateWindowSizeDependentResources()
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.
void App1Main::Update()
{

	//give the system a cycle to sync all resources
	//if (hasRunOnce < 1) {
		//hasRunOnce++;
		//return;
	//}

	// Update scene objects.
	m_timer.Tick([&]()
		{
			
			double leftStickX   = 0.0;
			double leftStickY   = 0.0;
			double rightStickX  = 0.0;
			double rightStickY  = 0.0;
			double leftTrigger  = 0.0;
			double rightTrigger = 0.0;

			const float deadzoneRadius = 0.2f;


	//clear controller debug text
	controllerDebugText = "";
	
	numControllers = myRawGameControllers.size();
	
	//get the selected controllers index 
	controllerDebugText += "Selected controller Index: " + to_string(selectedController) + newLine;

	//is there a connected controller and it is not keyboard and mouse
	if (numControllers > 0 && selectedController != -1) {

		// loop through each connected controller object
		for (DWORD i = 0; i < numControllers; i++)
		{
			gControllerData[i]->Poll(i);
		}
		
		int j = selectedController;
		//set data based on indexed controller
		{
			leftStickX = gControllerData[j]->leftStickX;
			leftStickY = gControllerData[j]->leftStickY;
			rightStickX = gControllerData[j]->rightStickX;
			rightStickY = gControllerData[j]->rightStickY;

			controllerDebugText += "Controller " + to_string(j) + ": " + gControllerData[j]->cName;
			controllerDebugText += newLine + "Button Count: " + to_string(gControllerData[j]->ButtonCount);
			controllerDebugText += newLine + "HardWare ID: " + to_string(gControllerData[j]->HWVendor);
			controllerDebugText += newLine;
		}



		//update camera if user used a control stick
					//update the camera controls - keyboard and mouse
		//Camera1->ForwardUnits = 0.0;
		//Camera1->SidewardUnits = 0.0;
		//if (upKeyPressed) {
		//	Camera1->MoveCamera(0.1f);
		//	Camera1->ForwardUnits = 0.1f;

		//}
		//if (downKeyPressed) {
		//	Camera1->MoveCamera(-0.1f);
		//	Camera1->ForwardUnits = -0.1f;

		//}
		//if (leftKeyPressed) {
		//	Camera1->StrafeCamera(-0.1f);
		//	Camera1->SidewardUnits = -0.1f;
		//}
		//if (rightKeyPressed) {
		//	Camera1->StrafeCamera(0.1f);
		//	Camera1->SidewardUnits = 0.1f;
		//}

		//move camera with the left thumb stick
		// Choose a deadzone. Readings inside this radius are ignored.

		{
			
			// Input accepted, process it.
			//set camera position
			if (abs(leftStickX) > deadzoneRadius) {
				float moveSpeed = leftStickX * 0.05;
				Camera1->MoveCamera(moveSpeed);
			}
			if (abs(leftStickY) > deadzoneRadius) {
				float moveSpeed = leftStickY * 0.05;
				Camera1->StrafeCamera(moveSpeed);
			}
			//set camara view 
			if (abs(rightStickX) > deadzoneRadius) {
				float moveSpeed = rightStickX * 0.1;
				Camera1->SetPitch(moveSpeed);                   
			}
			if (abs(rightStickY) > deadzoneRadius) {
				float moveSpeed = rightStickY * 0.1;
				Camera1->SetYaw(moveSpeed);                     
			}
		}
	}//end controller check


	//if (selectedController == -1 ) {
	//	Camera1->ForwardUnits = 0.0;
	//	Camera1->SidewardUnits = 0.0;
	//	if (upKeyPressed) {
	//		Camera1->MoveCamera(0.1f);
	//		Camera1->ForwardUnits = 0.1f;

	//	}
	//	if (downKeyPressed) {
	//		Camera1->MoveCamera(-0.1f);
	//		Camera1->ForwardUnits = -0.1f;

	//	}
	//	if (leftKeyPressed) {
	//		Camera1->StrafeCamera(-0.1f);
	//		Camera1->SidewardUnits = -0.1f;
	//	}
	//	if (rightKeyPressed) {
	//		Camera1->StrafeCamera(0.1f);
	//		Camera1->SidewardUnits = 0.1f;
	//	}

	//}



	// TODO: Replace this with your app's content update functions.
//		auto m_localCollection = ref new Vector<RawGameController^>();


//		

//auto gamecontrollers = RawGameController::RawGameControllers;
//for (auto gamecontroller : gamecontrollers)
//{
//	m_localCollection->Append(gamecontroller);
//}

//get the name of the controller
//if (myRawGameControllers.size() == 1) {
//	auto type = myRawGameControllers[0].GetAt(0);
//	
//	auto btnCount = type->ButtonCount;
//	
//	uint16_t  HWPid = type->HardwareProductId;
//	uint16_t  HWVendor = type->HardwareVendorId;
//	
//	int b = 0;
//}



	m_fpsTextRenderer->Update(m_timer);

	m_fpsTextRenderer->debugText = "";

	std::string pitch = std::to_string(Camera1->GetCameraPitch());
	std::string yaw = std::to_string(Camera1->GetCameraYaw());
	std::string camPosX = std::to_string(Camera1->GetCameraPositionX());
	std::string camPosY = std::to_string(Camera1->GetCameraPositionY());
	std::string camPosZ = std::to_string(Camera1->GetCameraPositionZ());

	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + "Camera Pitch: " + pitch;
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + "Camera Yaw: " + yaw;
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + "Camera Position X: " + camPosX;
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + "Camera Position Y: " + camPosY;
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + "Camera Position Z: " + camPosZ;
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine;

	//int numControllers = myRawGameControllers.size();
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + "Game Controller Data";
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + "Number of Controlers: " + to_string(numControllers);
	m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine;
	m_fpsTextRenderer->debugText += controllerDebugText;


		});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool App1Main::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}





	XMMATRIX view = Camera1->GetCameraViewMatrix();
	m_sceneRenderer->UpdateCameraView(view);

	//XMMATRIX view = inCamera.GetCameraViewMatrix();
	//m_sceneRenderer->UpdateCameraView(view);

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView* const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.


			//draw the ground plane
	m_sceneRenderer->Update(m_timer);
	m_sceneRenderer->Render();
	//draw the rotating cube
	m_sceneRenderer->UpdateModel1(m_timer);
	m_sceneRenderer->Render();




	//render the text to screen
	m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void App1Main::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void App1Main::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

