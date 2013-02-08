#include "stdafx.h"
#include "Camera.h"

using namespace Scattering;
using namespace DirectX;


const XMVECTOR Camera::DEFAULT_POSITION = XMVectorSet( 0, 0, 0, 0 );
const XMVECTOR Camera::DEFAULT_LOOKAT = XMVectorSet( 0, 0, 1, 0 );
const XMVECTOR Camera::DEFAULT_UP = XMVectorSet( 0, 1, 0, 0 );


/* Constructor */
Camera::Camera()
    : _position( DEFAULT_POSITION ),
      _lookAt( DEFAULT_LOOKAT ),
      _up( DEFAULT_UP )
{ }


/* Constructor */
Camera::Camera( DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 lookAt )
    : _position( XMLoadFloat3( &position ) ),
      _lookAt( XMLoadFloat3( &lookAt ) ),
      _up( DEFAULT_UP )
{ }


/* Destructor */
Camera::~Camera(void) { }


/* Changes the pitch of the camera by the specified delta in radians */
void Camera::pitch( float dy ) {
    pitchDegrees( XMConvertToRadians( dy ) );
}


/* Changes the pitch of the camera by the specified delta in degrees */
void Camera::pitchDegrees( float dy ) {
    applyTransformation( XMMatrixRotationY( dy ) );
}


/* Changes the yaw of the camera (rotation around y axis) in radians */
void Camera::yaw( float dx ) {
    yawDegrees( XMConvertToRadians( dx ) );
}


/* Changes the yaw of the camera (rotation around y axis) in degrees */
void Camera::yawDegrees( float dx ) {
    applyTransformation( XMMatrixRotationY( dx ) );
}


/* Applys the transformation matrix to the camera vectors */
void Camera::applyTransformation( DirectX::XMMATRIX transform ) {
    _position = XMVector3Transform(_position, transform );
    _lookAt = XMVector3Transform(_lookAt, transform );
    _up = XMVector3Transform(_up, transform );
}


/* Returns the view matrix based on the camera axes */
XMMATRIX Camera::getViewMatrix() {
    return XMMatrixLookAtLH( _position, _lookAt, _up );
}