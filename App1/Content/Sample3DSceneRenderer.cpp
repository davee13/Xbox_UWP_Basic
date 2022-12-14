#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace App1;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	//world object data - set to default of zero
	ScalingOrigin = { 0,0,0 };
	Scaling = { 0,0,0 };
	RotationOrigin = { 0,0,0 };
	RotationQuaternion = { 0,0,0 };
	Translation = { 0,0,0 };
	UV = { 1,1 };
	
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();


	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState* rasterizerState;

	auto device = m_deviceResources->GetD3DDevice();

	auto context = m_deviceResources->GetD3DDeviceContext();

	//ID3D11RasterizerState** m_rasterState = nullptr;
device->CreateRasterizerState(&rasterDesc, &rasterizerState);

	context->RSSetState(rasterizerState);

	
	
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}
	
	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));






}


// Called once per frame, rotates the cube and calculates the model and view matrices.
void App1::Sample3DSceneRenderer::Update(DX::StepTimer const& timer, int index)
{
	if (!m_tracking)
	{

		//update the mesh object  -index/vertex  buffers and  index data
		m_indexCount = ModelData[index]->MeshData.m_indexCount;
		m_vertexBuffer = ModelData[index]->MeshData.m_vertexBuffer;
		m_indexBuffer = ModelData[index]->MeshData.m_indexBuffer;

		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		//get world SRT data
		float PosX = ModelData[index]->Position.x;
		float PosY = ModelData[index]->Position.y;
		float PosZ = ModelData[index]->Position.z;

		float scaleX = ModelData[index]->Scale.x;
		float scaleY = ModelData[index]->Scale.y;
		float scaleZ = ModelData[index]->Scale.z;

		float rotX = ModelData[index]->Rotation.y;
		float rotY = ModelData[index]->Rotation.x;
		float rotZ = ModelData[index]->Rotation.z;

		PositionObject(PosX,PosY, PosZ);
		ScaleObject(scaleX, scaleY, scaleZ);
		RotateObject(rotX, rotY, rotZ);
		ComputeWorldMatrix();


		//set uv data
		UV = ModelData[index]->UV;


		ID3D11Buffer* g_pConstantBuffer11 = NULL;
		__declspec(align(16))
			struct VS_CONSTANT_BUFFER
		{
			DirectX::XMFLOAT2 UV;
			//float2 UV;
		};


		VS_CONSTANT_BUFFER VsConstData;
		//VsConstData.UV = 1.0;
		VsConstData.UV.x = UV.x;
		VsConstData.UV.y = UV.y;


		// Fill in a buffer description.
	// Fill in a buffer description.
		D3D11_BUFFER_DESC cbDesc;
		ZeroMemory(&cbDesc, sizeof(cbDesc));
		cbDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		// Fill in the subresource data.
	// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &VsConstData;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		HRESULT result = g_deviceResources->GetD3DDevice()->CreateBuffer(&cbDesc, &InitData, &g_pConstantBuffer11);

		//g_deviceResources->GetD3DDeviceContext()->UpdateSubresource(g_pConstantBuffer11, 0, 0, &cbDesc, 0, 0);

		// Now set the constant buffer in the vertex shader with the updated values.
		g_deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(1, 1, &g_pConstantBuffer11);


	}
}



// Position the cube in 3d space
void Sample3DSceneRenderer::PositionObject(float X, float Y, float Z)
{
	Translation.X = X;
	Translation.Y = Y;
	Translation.Z = Z;
}

// Position the cube in 3d space
void Sample3DSceneRenderer::ScaleObject(float X, float Y, float Z)
{
	Scaling.X = X;
	Scaling.Y = Y;
	Scaling.Z = Z;
}

// Position the cube in 3d space
void Sample3DSceneRenderer::RotateObject(float X, float Y, float Z)
{
	RotationOrigin.X = X;
	RotationOrigin.Y = Y;
	RotationOrigin.Z = Z;
}

// compute final World Matrix
void Sample3DSceneRenderer::ComputeWorldMatrix()
{
	// Prepare to pass the updated model matrix to the shader
	FXMVECTOR ScalingOrigin1{ ScalingOrigin.X,ScalingOrigin.Y,ScalingOrigin.Z };
	FXMVECTOR ScalingOrientationQuaternion1{ ScalingOrientationQuaternion.X,ScalingOrientationQuaternion.Y,ScalingOrientationQuaternion.Z };
	FXMVECTOR Scaling1{ Scaling.X,Scaling.Y,Scaling.Z };
	HXMVECTOR Translation1{ Translation.X,Translation.Y,Translation.Z };
	//GXMVECTOR RotationOrigin1{ RotationOrigin.X, RotationOrigin.Y, RotationOrigin.Z };
	//HXMVECTOR RotationQuaternion1{ RotationQuaternion.X,RotationQuaternion.Y,RotationQuaternion.Z };
	GXMVECTOR RotationOrigin1{ 0,0,0 };
	HXMVECTOR RotationQuaternion1{ 0,0,0};


	XMMATRIX World = XMMatrixTransformation(ScalingOrigin1, ScalingOrientationQuaternion1, Scaling1, RotationOrigin1, RotationQuaternion1, Translation1);
	World = XMMatrixTranspose(World);

	//apply rotation
	XMMATRIX RotationX = XMMatrixTranspose(XMMatrixRotationX(RotationOrigin.X));
	XMMATRIX RotationY = XMMatrixTranspose(XMMatrixRotationY(RotationOrigin.Y));
	XMMATRIX RotationZ = XMMatrixTranspose(XMMatrixRotationZ(RotationOrigin.Z));


	World = World *  (RotationX * RotationY * RotationZ);
	//commit to vertex shader
	XMStoreFloat4x4(&m_constantBufferData.world, World);

}




void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		//float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		//Rotate(radians);
	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render( )
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	//update camera
	auto device = m_deviceResources->GetD3DDevice();

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);




	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}


//update view matrix for shaders
void App1::Sample3DSceneRenderer::UpdateCameraView(XMMATRIX inView)
{
	//XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData.view, inView);
}





void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
				)
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
				)
			);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});

	// Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this]()
		{

	//		MeshObject cube;
	//cube = createCube();

	//m_indexCount = cube.m_indexCount;
	//m_vertexBuffer = cube.m_vertexBuffer;
	//m_indexBuffer = cube.m_indexBuffer;



		});



		//// Load mesh vertices. Each vertex has a position and a color.
		//static const VertexPositionColor cubeVertices[] = 
		//{
		//	{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
		//	{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
		//	{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
		//	{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
		//	{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
		//	{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
		//	{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
		//	{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
		//};

		//D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		//vertexBufferData.pSysMem = cubeVertices;
		//vertexBufferData.SysMemPitch = 0;
		//vertexBufferData.SysMemSlicePitch = 0;
		//CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
		//DX::ThrowIfFailed(
		//	m_deviceResources->GetD3DDevice()->CreateBuffer(
		//		&vertexBufferDesc,
		//		&vertexBufferData,
		//		&m_vertexBuffer
		//		)
		//	);

		//// Load mesh indices. Each trio of indices represents
		//// a triangle to be rendered on the screen.
		//// For example: 0,2,1 means that the vertices with indexes
		//// 0, 2 and 1 from the vertex buffer compose the 
		//// first triangle of this mesh.
		//static const unsigned short cubeIndices [] =
		//{
		//	0,2,1, // -x
		//	1,2,3,

		//	4,5,6, // +x
		//	5,7,6,

		//	0,1,5, // -y
		//	0,5,4,

		//	2,6,7, // +y
		//	2,7,3,

		//	0,4,6, // -z
		//	0,6,2,

		//	1,3,7, // +z
		//	1,7,5,
		//};

		//m_indexCount = ARRAYSIZE(cubeIndices);

		//D3D11_SUBRESOURCE_DATA indexBufferData = {0};
		//indexBufferData.pSysMem = cubeIndices;
		//indexBufferData.SysMemPitch = 0;
		//indexBufferData.SysMemSlicePitch = 0;
		//CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		//DX::ThrowIfFailed(
		//	m_deviceResources->GetD3DDevice()->CreateBuffer(
		//		&indexBufferDesc,
		//		&indexBufferData,
		//		&m_indexBuffer
		//		)
		//	);


		//m_loadingComplete = true;
	//}//);

	// Once the cube is loaded, the object is ready to be rendered.
	createCubeTask.then([this] () {
		m_loadingComplete = true;
	});
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

