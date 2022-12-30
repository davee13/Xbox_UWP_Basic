#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <pch.h>


using namespace DirectX;



extern class Camera
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


extern Camera* Camera1;  //this is the main camera that will be used throughout the game


#endif