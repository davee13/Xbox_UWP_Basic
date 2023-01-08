﻿#include "pch.h"
#include "App1Main.h"
#include "Common\DirectXHelper.h"


//
#include <collection.h>

//
//using namespace Platform;
//using namespace Windows::Foundation;
//using namespace Windows::Gaming::Input;
using namespace App1;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace Platform::Collections;

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

	g_deviceResources = m_deviceResources;

	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/


	//init the camera and input controls
	Camera1 = new Camera();
	iMouse = new MouseData();
	iKeyboard = new KeyboardData();

	Camera1->PositionCamera(-5, 0, 0);


	//load a texture .dds


	ID3D11Device* d3dDevice = deviceResources->GetD3DDevice();
	HRESULT hr = CreateDDSTextureFromFile(d3dDevice,L"brick.dds", &iTexture, &iTextureView, 0, nullptr);

	// Create a texture sampler state description.
	ID3D11SamplerState* m_sampleState;
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	CD3D11_SHADER_RESOURCE_VIEW_DESC pDesc1;
	iTextureView->GetDesc(&pDesc1);
	
	// Create the texture sampler state.
	HRESULT result = d3dDevice->CreateSamplerState(&samplerDesc, &m_sampleState);

	
	// Set shader texture resource in the pixel shader.
	deviceResources->GetD3DDeviceContext()->PSSetShaderResources(0, 1, &iTextureView);




	//create the new objects class
	//ground plane
	ModelData.push_back(new CObjects);
	ModelData[0]->setMeshData(createCube());
	ModelData[0]->Position.y = -1.0;
	ModelData[0]->Scale = { 1000,1,1000 };
	ModelData[0]->UV.x = 100;
	ModelData[0]->UV.y = 100;

	//world center
	ModelData.push_back(new CObjects);
	ModelData[1]->setMeshData(createCube());
	ModelData[1]->Scale = { 1,1,1 };

	//world corners
	ModelData.push_back(new CObjects);
	ModelData[2]->setMeshData(createCube());
	ModelData[2]->Position.x = 500.0;
	ModelData[2]->Position.z = 500.0;
	ModelData[2]->Scale = { 1,100,1 };
	//world corners
	ModelData.push_back(new CObjects);
	ModelData[3]->setMeshData(createCube());
	ModelData[3]->Position.x = -500.0;
	ModelData[3]->Position.z = -500.0;
	ModelData[3]->Scale = { 1,100,1 };
	//world corners
	ModelData.push_back(new CObjects);
	ModelData[4]->setMeshData(createCube());
	ModelData[4]->Position.x = -500.0;
	ModelData[4]->Position.z = 500.0;
	ModelData[4]->Scale = { 1,100,1 };
	//world corners
	ModelData.push_back(new CObjects);
	ModelData[5]->setMeshData(createCube());
	ModelData[5]->Position.x = 500.0;
	ModelData[5]->Position.z = -500.0;
	ModelData[5]->Scale = { 1,100,1 };





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

			updateInputDevices();


	


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

	ID3D11Device* d3dDevice = g_deviceResources->GetD3DDevice();
	// Create a texture sampler state description.
	ID3D11SamplerState* m_sampleState;
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//samplerDesc.BorderColor[0] = 0;
	//samplerDesc.BorderColor[1] = 0;
	//samplerDesc.BorderColor[2] = 0;
	//samplerDesc.BorderColor[3] = 0;
	//samplerDesc.MinLOD = 0;
	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	CD3D11_SHADER_RESOURCE_VIEW_DESC pDesc1;
	iTextureView->GetDesc(&pDesc1);

	// Create the texture sampler state.
	HRESULT result = d3dDevice->CreateSamplerState(&samplerDesc, &m_sampleState);


	// Set the sampler state in the pixel shader.
	g_deviceResources->GetD3DDeviceContext()->PSSetSamplers(0, 1, &m_sampleState);
	// Set shader texture resource in the pixel shader.
	g_deviceResources->GetD3DDeviceContext()->PSSetShaderResources(0, 1, &iTextureView);


	ID3D11Buffer* g_pConstantBuffer11 = NULL;

	struct VS_CONSTANT_BUFFER
	{
		DirectX::XMFLOAT2 UV;
	} ;
	
	
	VS_CONSTANT_BUFFER VsConstData;
	//VsConstData.UV = 1.0;
	VsConstData.UV.x = 1000;
	VsConstData.UV.y = 1000;


	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.ByteWidth = sizeof(VsConstData);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &VsConstData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = d3dDevice->CreateBuffer(&cbDesc, &InitData, &g_pConstantBuffer11);

	//g_deviceResources->GetD3DDeviceContext()->UpdateSubresource(g_pConstantBuffer11, 0, 0, &cbDesc, 0, 0);


	//UINT stride = sizeof(ModelViewProjectionConstantBuffer);
	//UINT offset = sizeof(VertexUV);
	// Now set the constant buffer in the vertex shader with the updated values.
	g_deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(1, 1, &g_pConstantBuffer11);



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

	//loop over each world object to render
	for (int i = 0; i < ModelData.size(); i++) {
		m_sceneRenderer->Update(m_timer, i);
		m_sceneRenderer->Render();
	}




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

