#pragma once

#include "stdafx.h"

namespace Scattering {

class Direct3DWindow {
public:

    /* Constructor */
    Direct3DWindow(HINSTANCE hinstance);

    /* Destructor */
    ~Direct3DWindow(void);
	
    /* Main application loop */
	int start();
 
    /* Handles resizing of the client window */
	virtual void onResize(); 

    /* Updates the items in the scene at each tick */
	virtual void updateScene();

    /* Renders the current scene to the window */
	virtual void drawScene(); 

    /* Handle Windows window messages */
	virtual LRESULT msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Calculate the aspect ratio of the window
	float aspectRatio() const;

private:

    /* Initialize the main application window */
    void createWindow();

    /* Initialize Direct3D objects */
    void setupDirect3D();

	HINSTANCE _hinstance;
	HWND _hwnd;

    // Flags for managing the window
	bool _paused, _minimized, _maximized, _resizing;

    // Direct3D objects
	ID3D11Device* _d3dDevice;
	ID3D11DeviceContext* _d3dDeviceContext;
	IDXGISwapChain* _swapChain;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;
	D3D11_VIEWPORT _viewport;

	int _width;
	int _height;
};

}