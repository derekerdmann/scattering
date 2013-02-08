#include "stdafx.h"
#include "Direct3DWindow.h"

using namespace Scattering;
using namespace DirectX;

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
      _width( 600 ),
      _height( 600 ),
      // Planet sizes given in meters
      _planet( 1, 1 ),
      //_planet( 6371000, 100000 )
      _camera( XMFLOAT3( 5, 0, 0 ), XMFLOAT3( 0, 0, 0 ) ),
      _lastMousePosition( 0, 0 )
{
    window = this;

    createWindow();
    setupDirect3D();

    setScene();
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
	MSG msg = {0};
 
	while(msg.message != WM_QUIT) {

		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE )) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );

		// Otherwise, do animation/game stuff.
		} else {	
			if( !_paused ) {
				updateScene();	
				drawScene();
			} else {
				Sleep(100);
			}
        }
    }

	return (int) msg.wParam;
}


/* Handles resizing of the client window */
void Direct3DWindow::onResize() {

	assert( _d3dDeviceContext );
	assert( _d3dDevice );
	assert( _swapChain );

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	Release( _renderTargetView );
	Release( _depthStencilView );
	Release( _depthStencilBuffer );


	// Resize the swap chain and recreate the render target view.

    HRESULT hr = S_OK;

	hr = _swapChain->ResizeBuffers(1, _width, _height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    assert( SUCCEEDED( hr ) );

	ID3D11Texture2D* backBuffer;
	hr = _swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    assert( SUCCEEDED( hr ) );

	hr = _d3dDevice->CreateRenderTargetView(backBuffer, 0, &_renderTargetView);
    assert( SUCCEEDED( hr ) );

	Release( backBuffer );

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width = _width;
	depthStencilDesc.Height = _height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags = 0;

	hr = _d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &_depthStencilBuffer);
    assert( SUCCEEDED( hr ) );

	hr = _d3dDevice->CreateDepthStencilView(_depthStencilBuffer, 0, &_depthStencilView);
    assert( SUCCEEDED( hr ) );


	// Bind the render target view and depth/stencil view to the pipeline.
	_d3dDeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    // TODO - figure out if this is needed
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = false;
	rasterDesc.DepthBiasClamp = 0;
	rasterDesc.DepthClipEnable = false;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0;

    hr = _d3dDevice->CreateRasterizerState( &rasterDesc, &_rasterState );
    assert( SUCCEEDED( hr ) );

    _d3dDeviceContext->RSSetState( _rasterState );
	

	// Set the viewport transform.
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0;
	_viewport.MaxDepth = 1;

	_d3dDeviceContext->RSSetViewports(1, &_viewport);

	// The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio(), 1.0f, 1000.0f);
    P = XMMatrixOrthographicLH( 3, 3, 1, 100 );
	XMStoreFloat4x4(&_proj, XMMatrixTranspose( P ) );
}


/* Updates the items in the scene at each tick */
void Direct3DWindow::updateScene() {

}


/* Renders the current scene to the window */
void Direct3DWindow::drawScene() {
	assert( _d3dDeviceContext );
	assert( _swapChain );

	DirectX::XMVECTORF32 Background = {1.0f, 1.0f, 1.0f, 1.0f};

	_d3dDeviceContext->ClearRenderTargetView(
        _renderTargetView,
        reinterpret_cast<const float*>(&Background)
    );

	_d3dDeviceContext->ClearDepthStencilView(
        _depthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f,
        0
    );

	// Set constants
	XMMATRIX world = XMMatrixIdentity();
    XMMATRIX view  = _camera.getViewMatrix();
	XMMATRIX proj  = XMLoadFloat4x4( &_proj );
	XMMATRIX worldViewProj = world * view * proj;

    XMFLOAT4X4 wvp;
    XMStoreFloat4x4( &wvp, worldViewProj );

    // Fill in a buffer description.
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof( XMFLOAT4X4 );
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = &wvp;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    // Create the buffer.
    ID3D11Buffer *constantBuffer = nullptr;
    HRESULT hr = _d3dDevice->CreateBuffer(
        &cbDesc,
        &data,
        &constantBuffer
    );

    assert( SUCCEEDED( hr ) );

    // Set the buffer.
    _d3dDeviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer );
    Release( constantBuffer );

    _planet.draw( _d3dDevice, _d3dDeviceContext );

	_swapChain->Present(0, 0);
}


/* Handle Windows window messages */
LRESULT Direct3DWindow::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch( msg ) {

	case WM_MOUSEMOVE:
		onMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


// Calculate the aspect ratio of the window
float Direct3DWindow::aspectRatio() const {
	return static_cast<float>(_width) / _height;
}

void Direct3DWindow::onMouseMove(WPARAM buttonState, int x, int y)
{
	if( (buttonState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
        float dx = 0.025f * static_cast<float>( x - _lastMousePosition.x );
		float dy = 0.025f * static_cast<float>( y - _lastMousePosition.y );

		_camera.pitchDegrees(dy);
        _camera.yawDegrees(dx);
	}

	_lastMousePosition.x = x;
	_lastMousePosition.y = y;
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

    assert( SUCCEEDED( hr ) );

	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);
}


/* Initialize Direct3D objects */
void Direct3DWindow::setupDirect3D() {

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags, 
        nullptr, 0, // Use whatever feature level is available
        D3D11_SDK_VERSION,
        &_d3dDevice,
        &featureLevel,
        &_d3dDeviceContext
    );

	assert( SUCCEEDED( hr ) );
	assert( featureLevel == D3D_FEATURE_LEVEL_11_0 );

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count   = 1;
    sd.SampleDesc.Quality = 0;

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = _hwnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = nullptr;
	hr = S_OK;

    hr = _d3dDevice->QueryInterface( IID_PPV_ARGS( &dxgiDevice ) );
    assert( SUCCEEDED( hr ) );
	      
	IDXGIAdapter* dxgiAdapter = nullptr;
	hr = dxgiDevice->GetParent( IID_PPV_ARGS( &dxgiAdapter ) );
    assert( SUCCEEDED( hr ) );

	IDXGIFactory* dxgiFactory = nullptr;
	hr = dxgiAdapter->GetParent( IID_PPV_ARGS( &dxgiFactory) );
    assert( SUCCEEDED( hr ) );

	hr = dxgiFactory->CreateSwapChain( _d3dDevice, &sd, &_swapChain);
    assert( SUCCEEDED( hr ) );
	
	Release( dxgiDevice );
	Release( dxgiAdapter );
	Release( dxgiFactory );

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.
	
	onResize();
}


/* Set the scene */
void Direct3DWindow::setScene() {
    _planet.createBuffer( _d3dDevice );
    _planet.setupShaders( _d3dDevice, _d3dDeviceContext );
}