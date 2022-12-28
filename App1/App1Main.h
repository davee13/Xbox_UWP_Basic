#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"


using namespace DirectX;

// Renders Direct2D and 3D content on the screen.
namespace App1
{



	class Camera
	{
	public:
		Camera();
		~Camera();
	private:

		float Pitch, Yaw;
		XMVECTOR Position;
		
		XMVECTOR Target;
		XMVECTOR Up;
		XMVECTOR LookVector;

		XMVECTOR Velocity;
		XMMATRIX  ViewMatrix;


	public:
		float ForwardUnits;
		float SidewardUnits;
		XMVECTOR MoveVector;
		void PositionCamera(float PosX, float PosY, float PosZ);
		void PositionCameraX(float PosX);
		void PositionCameraY(float PosY);
		void PositionCameraZ(float PosZ);
		void SetPitch(float NewPitch);
		void SetStaticPitch(float NewPitch);
		void SetYaw(float NewYaw);
		void SetStaticYaw(float NewYaw);


		void UpdateCamera();
		void UpdateViewMatrix();
		void UpdateCameraReflection(float ForwardUnits, float SidewardUnits);
		void MoveCamera(float Units);
		void StrafeCamera(float Units);
		void SetTarget(float TargetX, float TargetY, float TargetZ);
		void SetTargetVector(XMVECTOR NewTarget);
		void SetViewVector(XMVECTOR NewView);
		void PointCamera(float x, float y, float z);
		void setUpDirection(XMVECTOR newUp);

		//return functions
		float GetCameraPositionX();
		float GetCameraPositionY();
		float GetCameraPositionZ();
		XMVECTOR GetCameraPositionXYZ();

		float GetCameraPitch();
		float GetCameraYaw();

		XMMATRIX GetCameraViewMatrix();
		XMVECTOR GetViewVector();
		XMVECTOR GetTargetVector();

		//physx specific returns
		float GetVelocityX();
		float GetVelocityY();
		float GetVelocityZ();

	};









	class App1Main : public DX::IDeviceNotify
	{
	public:
		App1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~App1Main();
		void CreateWindowSizeDependentResources();
		void Update( Camera inCamera );
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		//float m_Pitch;
		Camera Camera1;

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;
		
	};
}