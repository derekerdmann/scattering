#include "stdafx.h"
#include "Direct3DWindow.h"

using namespace Scattering;


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
LRESULT Direct3DWindow::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return 0;
}


// Calculate the aspect ratio of the window
float Direct3DWindow::aspectRatio() const {
	return static_cast<float>(_width) / _height;
}



/* Releases a com pointer */
inline void Release( IUnknown *object ) {
    if( object ) {
        object->Release();
    }
}


/* Initialize the main application window */
void Direct3DWindow::createWindow() {

}


/* Initialize Direct3D objects */
void Direct3DWindow::setupDirect3D() {

}