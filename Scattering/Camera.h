#pragma once
#include "stdafx.h"

namespace Scattering {

/* Controls the view */
class Camera {
public:

    /* Default lookAt vector */
    static const DirectX::XMVECTOR Camera::DEFAULT_LOOKAT;

    /* Default up vector */
    static const DirectX::XMVECTOR Camera::DEFAULT_UP;

    /* Default right vector */
    static const DirectX::XMVECTOR Camera::DEFAULT_RIGHT;


    /* Default constructor, defaults position to [0 0 0], lookAt to [0 0 1] */
    Camera();

    /* Constructor
     * position - the initial location of the camera 
     * lookAt - the initial target of the camera
     */
    Camera(
        DirectX::XMFLOAT3 position,
        DirectX::XMFLOAT3 lookAt,
        DirectX::XMFLOAT3 worldUp
    );

    /* Destructor */
    virtual ~Camera(void);

    /* Changes the pitch of the camera by the specified delta in radians */
    void pitch( float dy );

    /* Changes the pitch of the camera by the specified delta in degrees */
    void pitchDegrees( float dy );

    /* Changes the rotation around y axis in radians */
    void rotateY( float dx );

    /* Changes the rotation around y axis in degrees */
    void rotateYDegrees( float dx );

    /* Stores the position of the camera in the supplied XMFLOAT3 */
    void getPosition( DirectX::XMFLOAT4& position );

    /* Returns the view matrix based on the camera axes */
    DirectX::XMMATRIX getViewMatrix();

    /* Returns the projection matrix */
    DirectX::XMMATRIX getProjectionMatrix( float aspectRatio );

private:

    /* Vectors that determine the camera space */
    DirectX::XMVECTOR _position, _lookAt, _up, _right;

};

}