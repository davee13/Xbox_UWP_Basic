#pragma once
#include "pch.h"
#include "Camera.h"





Camera* Camera1;  //this is the main camera that will be used throughout the game




Camera::~Camera()
{
}

Camera::Camera()
{
	Pitch = 0.0f;
	Yaw = 0.0f;
	Position = XMVECTOR{ 0.0f, 1.0f, 0.0f };
	Target = XMVECTOR{ 0.0f, 0.0f, 0.0f };
	Up = XMVECTOR{ 0.0f, 1.0f, 0.0f };
	Velocity = XMVECTOR{ 0.0f, 0.0f, 0.0f };
	LookVector = (Target - Position);
	XMVector3Normalize(LookVector);

}


//position the camera
void Camera::PositionCamera(float PosX, float PosY, float PosZ)
{
	Position = { PosX,PosY,PosZ };
}
void Camera::PositionCameraX(float PosX)
{
	Position = { PosX,XMVectorGetY(Position),XMVectorGetZ(Position) };
}
void Camera::PositionCameraY(float PosY)
{
	Position = { XMVectorGetX(Position),PosY, XMVectorGetZ(Position) };
}
void Camera::PositionCameraZ(float PosZ)
{
	Position = { XMVectorGetX(Position),XMVectorGetY(Position), PosZ };
}


//get camera status
float Camera::GetCameraPitch()
{
	return Pitch;
}
//get camera yaw
float Camera::GetCameraYaw()
{
	return Yaw;
}
//get camera Position
XMVECTOR Camera::GetCameraPositionXYZ()
{
	return Position;
}
//get camera Position X
float Camera::GetCameraPositionX()
{
	return XMVectorGetX(Position);
}
//get camera Position Y
float Camera::GetCameraPositionY()
{
	return XMVectorGetY(Position);
}
//get camera Position Z
float Camera::GetCameraPositionZ()
{
	return XMVectorGetZ(Position);
}




//set pitch
void Camera::SetPitch(float NewPitch)
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

void Camera::MoveCamera(float Units)
{

	XMVECTOR vVector = Target - Position;	// Get the view vector
	// forward positive camera speed and backward negative camera speed.
	float Px = XMVectorGetX(Position);
	float Py = XMVectorGetY(Position);
	float Pz = XMVectorGetZ(Position);

	//New get x, z
	Px = Px + XMVectorGetX(vVector) * Units;
	Pz = Pz + XMVectorGetZ(vVector) * Units;

	Position = { Px,Py,Pz };

}

void Camera::StrafeCamera(float Units)
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