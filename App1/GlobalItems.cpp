#pragma once
#include "pch.h"
#include "GlobalItems.h"

using namespace App1;

std::shared_ptr<DX::DeviceResources> g_deviceResources = nullptr;

double mAccumulator = 0.0f;

ID3D11Resource* iTexture = nullptr;
ID3D11ShaderResourceView* iTextureView = nullptr;


std::vector<CObjects*> ModelData;




CObjects::CObjects()
{
	Position = { 0,0,0 };
	Scale = { 1,1,1 };
	Rotation = { 0,0,0 };
	MeshData.m_indexBuffer = nullptr;
	MeshData.m_vertexBuffer = nullptr;
	MeshData.m_indexCount = 0;
}

CObjects::~CObjects()
{
}

void CObjects::setMeshData( MeshObject inMeshData )
{
	MeshData.m_indexBuffer = inMeshData.m_indexBuffer;
	MeshData.m_vertexBuffer = inMeshData.m_vertexBuffer;
	MeshData.m_indexCount = inMeshData.m_indexCount;
}

MeshObject createCube()
{

	MeshObject cube;

	// Load mesh vertices. Each vertex has a position and a color.
	//static const VertexPositionColor cubeVertices[] =
	//{
	//	{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
	//	{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	//	{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	//	{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
	//	{XMFLOAT3(0.5f, -0.5f, -0.5f ), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	//	{XMFLOAT3(0.5f, -0.5f,  0.5f ), XMFLOAT3(1.0f, 0.0f, 1.0f)},
	//	{XMFLOAT3(0.5f,  0.5f, -0.5f ), XMFLOAT3(1.0f, 1.0f, 0.0f)},
	//	{XMFLOAT3(0.5f,  0.5f,  0.5f ), XMFLOAT3(1.0f, 1.0f, 1.0f)},
	//};


	static const VertexPositionColor cubeVertices[] =
	{

	    {XMFLOAT3(0.500000, -0.500000, -0.500000 ), XMFLOAT3(1.000000, 1.000000, 0.0f)},
		{XMFLOAT3(0.500000, 0.500000, -0.500000	 ), XMFLOAT3(1.000000, 0.000000, 0.0f)},
		{XMFLOAT3(-0.500000, 0.500000, -0.500000 ), XMFLOAT3(0.000000, 0.000000, 1.0f)},
		{XMFLOAT3(-0.500000, -0.500000, -0.500000), XMFLOAT3(0.000000, 1.000000, 1.0f)},
		{XMFLOAT3(0.500000, -0.500000, 0.500000	 ), XMFLOAT3(0.000000, 1.000000, 0.0f)},
		{XMFLOAT3(-0.500000, -0.500000, 0.500000 ), XMFLOAT3(1.000000, 1.000000, 1.0f)},
		{XMFLOAT3(-0.500000, 0.500000, 0.500000	 ), XMFLOAT3(1.000000, 0.000000, 0.0f)},
		{XMFLOAT3(0.500000, 0.500000, 0.500000	 ), XMFLOAT3(0.000000, 0.000000, 1.0f)},
		{XMFLOAT3(0.500000, 0.500000, -0.500000	 ), XMFLOAT3(1.000000, 1.000000, 0.0f)},
		{XMFLOAT3(0.500000, 0.500000, 0.500000	 ), XMFLOAT3(1.000000, 0.000000, 0.0f)},
		{XMFLOAT3(-0.500000, 0.500000, 0.500000	 ), XMFLOAT3(0.000000, 0.000000, 0.0f)},
		{XMFLOAT3(-0.500000, 0.500000, -0.500000 ), XMFLOAT3(0.000000, 1.000000, 0.0f)},
		{XMFLOAT3(0.500000, -0.500000, -0.500000 ), XMFLOAT3(1.000000, 0.000000, 1.0f)},
		{XMFLOAT3(-0.500000, -0.500000, -0.500000), XMFLOAT3(0.000000, 0.000000, 1.0f)},
		{XMFLOAT3(-0.500000, -0.500000, 0.500000 ), XMFLOAT3(0.000000, 1.000000, 0.0f)},
		{XMFLOAT3(0.500000, -0.500000, 0.500000	 ), XMFLOAT3(1.000000, 1.000000, 1.0f)},
		{XMFLOAT3(0.500000, 0.500000, -0.500000	 ), XMFLOAT3(0.000000, 0.000000, 0.0f)},
		{XMFLOAT3(0.500000, -0.500000, -0.500000 ), XMFLOAT3(0.000000, 1.000000, 0.0f)},
		{XMFLOAT3(-0.500000, -0.500000, -0.500000), XMFLOAT3(1.000000, 1.000000, 1.0f)},
		{XMFLOAT3(-0.500000, 0.500000, -0.500000 ), XMFLOAT3(1.000000, 0.000000, 0.0f)},

	};


	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	//DX::ThrowIfFailed(
		g_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&cube.m_vertexBuffer
		//)
	);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.
	//static const unsigned short cubeIndices[] =
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

		static const unsigned short cubeIndices[] =
		{
			2,1,0 ,
			0,3,2 ,
			6,5,4 ,
			4,7,6 ,
			10,9,8 ,
			8,11,10 ,
			14,13,12 ,
			12,15,14 ,
			16,9,15 ,
			15,17,16 ,
			19,18,14 ,
			14,10,19
		};




	cube.m_indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	//m_deviceResources->ThrowIfFailed(
	g_deviceResources->GetD3DDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&cube.m_indexBuffer
	);
	//);


	return cube;
}
