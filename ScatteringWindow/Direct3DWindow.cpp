#include "stdafx.h"
#include "Direct3DWindow.h"

using namespace Scattering;

namespace {

    // Global pointer is used to handle callback appropriately
	Direct3DWindow* window = nullptr;

}



/* Windows is expecting a standalone function, so this proxies to the Window object */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return window->msgProc(hwnd, msg, wParam, lParam);
}


/* Constructor */
Direct3DWindow::Direct3DWindow(HINSTANCE hinstance)
    : _hinstance( hinstance ),
	  _d3dDevice( nullptr ),
	  _d3dDeviceContext( nullptr ),
	  _swapChain( nullptr ),
	  _depthStencilBuffer( nullptr ),
	  _renderTargetView( nullptr ),
	  _depthStencilView( nullptr ),
	  _paused( false ),
      _minimized( false ),
      _maximized( false ),
      _resizing( false ),
      _width( 800 ),
      _height( 600 )
{
    window = this;

    createWindow();
    setupDirect3D();
}


/* Destructor
 * Release COM pointers in use
 */
Direct3DWindow::~Direct3DWindow(void) {
	Release( _renderTargetView );
	Release( _depthStencilView );
	Release( _swapChain );
	Release( _depthStencilBuffer );

	// Restore all default settings.
	if( _d3dDeviceContext ) {
		_d3dDeviceContext->ClearState();
    }

	Release( _d3dDeviceContext );
	Release( _d3dDevice );
}


/* Main application loop */
int Direct3DWindow::start() {
    return 0;
}


/* Handles resizing of the client window */
void Direct3DWindow::onResize() {

}


/* Updates the items in the scene at each tick */
void Direct3DWindow::updateScene() {

}


/* Renders the current scene to the window */
void Direct3DWindow::drawScene() {

}


/* Handle Windows window messages */
LRESULT Direct3DWindow::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


// Calculate the aspect ratio of the window
float Direct3DWindow::aspectRatio() const {
	return static_cast<float>(_width) / _height;
}



/* Releases a com pointer */
inline void Direct3DWindow::Release( IUnknown *object ) {
    if( object ) {
        object->Release();
    }
}


/* Initialize the main application window */
void Direct3DWindow::createWindow() {
    
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hinstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	auto registered = RegisterClass(&wc);
    assert( registered );

    /* Calculate the correct window size */
	RECT R = { 0, 0, _width, _height };
    AdjustWindowRect( &R, WS_OVERLAPPEDWINDOW, false );
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	_hwnd = CreateWindow(
        L"D3DWndClassName",
        L"Atmospheric Scattering", 
		WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width, height,
        0, 0,
        _hinstance,
        0
    ); 

    HRESULT hr = HRESULT_FROM_WIN32( GetLastError() );

    assert( hr == S_OK );

	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);
}


/* Initialize Direct3D objects */
void Direct3DWindow::setupDirect3D() {

}