#pragma once
#ifndef GLOBALITEMS_H
#define GLOBALITEMS_H
//#define NDEBUG

#include <pch.h>
#include "Camera.h"
#include "InputDevices.h"
#include "Utilities.h"
#include "DDSTextureLoader.h"


#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"

using namespace Platform::Collections;
using namespace std;
using namespace DirectX;
using namespace App1;

extern double mAccumulator;

extern ID3D11Resource* iTexture;
extern ID3D11ShaderResourceView* iTextureView;


extern std::shared_ptr<DX::DeviceResources> g_deviceResources;


struct MeshObject
{
	
	//vertex and index buffers
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;

	//index count
	uint32	m_indexCount;

	
};


extern MeshObject createCube();


struct float2 {
	float x;
	float y;
};
struct float3 {
	float x;
	float y;
	float z;
};
struct float4 {
	float x;
	float y;
	float z;
	float w;
};



extern class CObjects
{
public:
	CObjects();
	~CObjects();

	float3 Position;
	float3 Scale;
	float3 Rotation;

	MeshObject MeshData;



	void setMeshData(MeshObject inMeshData);

private:



};





extern std::vector<CObjects*> ModelData;










#endif