#pragma once
#include "stdafx.h"
#include "Observable.h"

namespace Scattering {

/* Listens for updates by Observable objects */
class Observer {
public:

    /* Constructor */
    Observer(void);

    /* Destructor */
    virtual ~Observer(void);

    /* Receives notifications from Observable objects */
    virtual void update( const Observable& source ) = 0;
};

}
