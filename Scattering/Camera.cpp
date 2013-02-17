#include "stdafx.h"
#include "Camera.h"

using namespace Scattering;
using namespace DirectX;


const XMVECTOR Camera::DEFAULT_LOOKAT = XMVectorSet( 0, 0, 1, 0 );


/* Constructor */
Camera::Camera(
    DirectX::XMFLOAT3 position,
    DirectX::XMFLOAT3 lookAt,
    XMFLOAT3 worldUp
) : _position( XMLoadFloat3( &position ) )
{
    _lookAt = XMVector3Normalize( XMLoadFloat3( &lookAt ) - _position ),
    _right = XMVector3Normalize( XMVector3Cross( XMLoadFloat3( &worldUp ), _lookAt ) ),
    _up = XMVector3Cross( _lookAt, _right );
}


/* Destructor */
Camera::~Camera(void) { }


/* Changes the pitch of the camera by the specified delta in radians */
void Camera::pitch( float dy ) {
    pitchDegrees( XMConvertToRadians( dy ) );
}


/* Changes the pitch of the camera by the specified delta in degrees */
void Camera::pitchDegrees( float dy ) {
    XMMATRIX R = XMMatrixRotationAxis( _right, dy );

    _up = XMVector3TransformNormal( _up, R );
    _lookAt = XMVector3TransformNormal( _lookAt, R );
}


/* Changes the rotateY of the camera (rotation around y axis) in radians */
void Camera::rotateY( float dx ) {
    rotateYDegrees( XMConvertToRadians( dx ) );
}


/* Changes the rotateY of the camera (rotation around y axis) in degrees */
void Camera::rotateYDegrees( float dx ) {
    XMMATRIX R = XMMatrixRotationY( dx );

	_right = XMVector3TransformNormal(_right, R);
	_up = XMVector3TransformNormal(_up, R);
	_lookAt = XMVector3TransformNormal(_lookAt, R);
}


/* Stores the position of the camera in the supplied XMFLOAT3 */
void Camera::getPosition( XMFLOAT4& pos ) {
    XMStoreFloat4( &pos, _position );
}


/* Returns the view matrix based on the camera axes */
XMMATRIX Camera::getViewMatrix() {

    // Ortho-normalize to prevent floating point error
	_lookAt = XMVector3Normalize( _lookAt );
	_up = XMVector3Normalize(XMVector3Cross( _lookAt, _right ) );
	_right = XMVector3Cross( _up, _lookAt ); 

	// Fill in the view matrix entries.
	float x = -XMVectorGetX( XMVector3Dot( _position, _right ) );
	float y = -XMVectorGetX( XMVector3Dot( _position, _up ) );
	float z = -XMVectorGetX( XMVector3Dot( _position, _lookAt ) );

    XMFLOAT3 R, U, L;
    XMStoreFloat3( &R, _right );
    XMStoreFloat3( &U, _up );
    XMStoreFloat3( &L, _lookAt );

    return XMMatrixSet(
        R.x, U.x, L.x, 0,
        R.y, U.y, L.y, 0,
        R.z, U.z, L.z, 0,
        x,   y,   z,   1
    );

}


/* Returns the projection matrix  */
XMMATRIX Camera::getProjectionMatrix( float aspectRatio ) {

	// The window resized, so update the aspect ratio and recompute the projection matrix.
    return XMMatrixPerspectiveFovLH( XM_PIDIV4, aspectRatio, 0.1f, 1000000 );
}