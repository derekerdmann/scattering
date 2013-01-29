#include "stdafx.h"
#include "Direct3DWindow.h"

using namespace Scattering;


/* Constructor */
Direct3DWindow::Direct3DWindow(HINSTANCE hinstance)
    : _hinstance( hinstance )
{
}


/* Destructor */
Direct3DWindow::~Direct3DWindow(void)
{
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


/* Initialize the main application window */
void Direct3DWindow::createWindow() {

}


/* Initialize Direct3D objects */
void Direct3DWindow::setupDirect3D() {

}