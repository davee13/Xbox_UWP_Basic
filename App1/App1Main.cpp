#include "pch.h"
#include "App1Main.h"
#include "Common\DirectXHelper.h"

using namespace App1;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

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
void App1Main::Update( Camera inCamera ) 
{

	
	// Update scene objects.
	m_timer.Tick([&]()
		{


			//XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));


		XMMATRIX view = inCamera.GetCameraViewMatrix();
		m_sceneRenderer->UpdateCameraView(view);
		// TODO: Replace this with your app's content update functions.
		



		m_fpsTextRenderer->Update(m_timer);
		
		m_fpsTextRenderer->debugText = "";

		

		//m_fpsTextRenderer->debugText = m_fpsTextRenderer->newLine + "Camera Position X: 0.0";

		std::string pitch = std::to_string(inCamera.GetCameraPitch());
		std::string yaw = std::to_string(inCamera.GetCameraYaw());

		m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + +"Camera Pitch: " + pitch;
		m_fpsTextRenderer->debugText += m_fpsTextRenderer->newLine + +"Camera Yaw: " + yaw;
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

	//XMMATRIX view = inCamera.GetCameraViewMatrix();
	//m_sceneRenderer->UpdateCameraView(view);

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
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



App1::Camera::~Camera()
{
}

App1::Camera::Camera()
{
	Pitch = 0.0f;
	Yaw = 0.0f;
	Position = XMVECTOR{ 0.0f, 1.0f, 0.0f };
	Target = XMVECTOR{ 0.0f, 0.0f, 0.0f };
	Up = XMVECTOR{0.0f, 1.0f, 0.0f};
	Velocity = XMVECTOR{ 0.0f, 0.0f, 0.0f };
	LookVector = (Target - Position);
	XMVector3Normalize(LookVector);
	
}



void App1::Camera::PositionCamera(float PosX, float PosY, float PosZ)
{
	Position = { PosX,PosY,PosZ };
}

float App1::Camera::GetCameraPitch()
{
	return Pitch;
}

//set pitch
void App1::Camera::SetPitch(float NewPitch)
{
	Pitch = Pitch + NewPitch;
}
void Camera::SetStaticPitch(float NewPitch)
{
	Pitch = NewPitch;

}
//set yaw
void Camera::SetYaw(float NewYaw)
{
	Yaw = Yaw + NewYaw;
}

void Camera::SetStaticYaw(float NewYaw)
{
	Yaw = NewYaw;
}
//get camera yaw
float Camera::GetCameraYaw()
{
	return Yaw;
}


//get view matrix
XMMATRIX Camera::GetCameraViewMatrix()
{
	return ViewMatrix;
}



void Camera::SetTarget(float TargetX, float TargetY, float TargetZ)
{
	XMVECTOR NewTarget{ TargetX, TargetY, TargetZ };
	Target = NewTarget;
}

void Camera::SetTargetVector(XMVECTOR NewTarget)
{
	Target = NewTarget;
}

void Camera::SetViewVector(XMVECTOR NewView)
{
	LookVector = NewView;
}

void Camera::UpdateViewMatrix()
{
	//D3DXMatrixLookAtLH(&ViewMatrix, &Position, &Target, &Up);
	ViewMatrix = XMMatrixTranspose(XMMatrixLookAtRH(Position, Target, Up));
}

void App1::Camera::MoveCamera(float Units)
{

	XMVECTOR vVector = Target - Position;	// Get the view vector
	// forward positive camera speed and backward negative camera speed.
	float Px = XMVectorGetX(Position);
	float Py = XMVectorGetY(Position);
	float Pz = XMVectorGetZ(Position);

	//New get x, z
	Px = Px + XMVectorGetX(vVector) * Units;
	Pz = Pz + XMVectorGetZ(vVector) * Units;

	Position={Px,Py,Pz};

}

void App1::Camera::StrafeCamera(float Units)
{
	XMVECTOR vVector = Target - Position;	// Get the view vector
	XMVECTOR vOrthoVector;              // Orthogonal vector for the view vector
	
	vOrthoVector = { -XMVectorGetZ(vVector) ,0,XMVectorGetX(vVector) };

	float Px = XMVectorGetX(Position);
	float Py = XMVectorGetY(Position);
	float Pz = XMVectorGetZ(Position);
	// left negative -cameraspeed and right positive +cameraspeed.
	//New get x, z
	Px = Px + XMVectorGetX(vOrthoVector) * Units;
	Pz = Pz + XMVectorGetZ(vOrthoVector) * Units;

	Position = { Px,Py,Pz };
}


void Camera::UpdateCamera()
{
	//Restrict the ability to look too high or too low
	if (Pitch < -1.56f)
		Pitch = -1.56f;
	if (Pitch > 1.56f)
		Pitch = 1.56f;

	if (Yaw >= 6.28f)
		Yaw = 0.0f;
	if (Yaw <= -6.28f)
		Yaw = 0.0f;



	//Set the target of the camera
	Target = XMVECTOR{ cosf(Pitch) * cosf(Yaw) * 10.0f, sinf(Pitch) * 10.0f, sinf(Yaw) * cosf(Pitch) * 10.0f } + Position;

	//Update the Look Vector
	//D3DXVec3Normalize(&LookVector, &(Target - Position));
	LookVector = (Target - Position);
	XMVector3Normalize(LookVector);


	//We do not want to move up or down so we zero the Y variable and only
	//move in the X and Z directions
	XMVECTOR XZLookVector{ XMVectorGetX(LookVector), 0.0f, XMVectorGetZ(LookVector) };
	//XZLookVector. = 0;
	//D3DXVec3Normalize(&XZLookVector, &XZLookVector);
	XMVector3Normalize(XZLookVector);

	

	XMVECTOR SideVector{ XMVectorGetZ(XZLookVector), 0.0f, -XMVectorGetX(XZLookVector) };
	//Velocity = (XZLookVector * ForwardUnits) + (SideVector * SidewardUnits);    //when physx is implemented take a look at this section
	//D3DXVec3Normalize(&MoveVector, &(D3DXVECTOR3((XZLookVector * ForwardUnits) + (SideVector * SidewardUnits))));

	MoveVector = ((XZLookVector * ForwardUnits) + (SideVector * SidewardUnits));

	XMVector3Normalize(MoveVector);

	//Update the View matix                                 |
	//D3DXMatrixLookAtLH(&ViewMatrix, &Position, &Target, &Up);
	//D3DDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
	ViewMatrix = XMMatrixTranspose(XMMatrixLookAtRH(Position, Target, Up));


	Target = Position + LookVector;

}