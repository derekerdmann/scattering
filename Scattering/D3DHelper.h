#pragma once

#include "stdafx.h"

namespace Scattering {

/* Releases a COM pointer if it exists */
inline void Release( IUnknown *object ) {
    if( object ){
        object->Release();
    }
}

}