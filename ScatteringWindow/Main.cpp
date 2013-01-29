#include "stdafx.h"
#include "Direct3DWindow.h"

using namespace Scattering;

/* Windows main method */
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE prevInstance,
    PSTR cmdLine,
    int showCmd
) {

	Direct3DWindow scattering(hInstance);
	return scattering.start();
}