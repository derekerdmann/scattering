#pragma once
#include "stdafx.h"

namespace Scattering {

/* Controls the view */
class Camera {
public:

    /* Constructor
     * "Up" vector defaults to [0 1 0]
     *
     * position - the initial location of the camera 
     * lookAt - the initial target of the camera
     */
    Camera( DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 lookAt );

    /* Destructor */
    virtual ~Camera(void);

    /* Changes the pitch of the camera by the specified delta in radians */
    void pitch( float dy );

    /* Changes the pitch of the camera by the specified delta in degrees */
    void pitchDegrees( float dy );

    /* Changes the yaw of the camera (rotation around y axis) in radians */
    void yaw( float dx );

    /* Changes the yaw of the camera (rotation around y axis) in degrees */
    void yawDegrees( float dx );

private:

    /* Applys the transformation matrix to the camera vectors */
    void applyTransformation( DirectX::XMMATRIX transform );

    /* Vectors that determine the camera space */
    DirectX::XMVECTOR _position, _lookAt, _up;

};

}