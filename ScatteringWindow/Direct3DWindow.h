#pragma once

#include "stdafx.h"

namespace Scattering {

class Direct3DWindow {
public:

    /* Constructor */
    Direct3DWindow(void);

    /* Destructor */
    ~Direct3DWindow(void);
	
	int Run();
 
	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.

	virtual bool Init();
	virtual void OnResize(); 
	virtual void UpdateScene(float dt)=0;
	virtual void DrawScene()=0; 
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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