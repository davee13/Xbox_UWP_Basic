#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "GlobalItems.h"

using namespace DirectX;

namespace App1
{
	struct gameVector {
		double X;
		double Y;
		double Z;
	};

	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		
		//update positon, scale, rotation
		void Update(DX::StepTimer const& timer, int index);
		
		
		//render the object
		void Render();
		void UpdateCameraView(XMMATRIX inView);
		void StartTracking();
		void TrackingUpdate(float positionX);
		void StopTracking();
		bool IsTracking() { return m_tracking; }

		float2 UV;

	private:
		
		void PositionObject(float X, float Y, float Z);
		void ScaleObject(float X, float Y, float Z);
		void RotateObject(float X, float Y, float Z);
		void ComputeWorldMatrix();


	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		//vertex and pixel shaders		
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;

		//buffer for shaders
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;
		ModelViewProjectionConstantBuffer     	m_constantBufferData;


		// Direct3D resources for mesh geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		uint32	m_indexCount;

		
		
		//control object world data
		gameVector ScalingOrigin;
		gameVector ScalingOrientationQuaternion;
		gameVector Scaling;
		gameVector RotationOrigin;
		gameVector RotationQuaternion;
		gameVector Translation;

		//Position, Scale Rotation
		//float3 pos;



		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;
	};
}

