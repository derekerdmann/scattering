#pragma once
#include "stdafx.h"
#include <vector>

namespace Scattering {

/* Forward declaration, defined in Observer.h */
class Observer;

/* Publishes updates to listeners when planetary models change */
class Observable {
public:

    /* Constructor */
    Observable(void);

    /* Destructor */
    virtual ~Observable(void);

    /* Adds the observer to future updates */
    void attach( Observer *observer );

    /* Removes the observer from future updates */
    void detach( Observer *observer );

    /* Broadcasts updates to all observers */
    void notify();

private:
    
    /* Observers that have registered with this object. Observers should never
     * be deleted without first being deregistered with the object. */
    std::vector<Observer *> _observers;

};
 
}