#include "stdafx.h"
#include "Camera.h"

using namespace Scattering;
using namespace DirectX;

/* Constructor */
Camera::Camera( DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 lookAt )
    : _up( XMVectorSet( 0, 1, 0, 0 ) )
{
}


/* Destructor */
Camera::~Camera(void) { }


/* Changes the pitch of the camera by the specified delta in radians */
void Camera::pitch( float dy ) {

}


/* Changes the pitch of the camera by the specified delta in degrees */
void Camera::pitchDegrees( float dy ) {

}


/* Changes the yaw of the camera (rotation around y axis) in radians */
void Camera::yaw( float dx ) {

}


/* Changes the yaw of the camera (rotation around y axis) in degrees */
void Camera::yawDegrees( float dx ) {

}


/* Applys the transformation matrix to the camera vectors */
void Camera::applyTransformation( DirectX::XMMATRIX transform ) {

}