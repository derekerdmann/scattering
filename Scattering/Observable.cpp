#include "stdafx.h"
#include "Observable.h"

using namespace Scattering;
using namespace std;


/* Constructor */
Observable::Observable(void) { }


/* Destructor */
Observable::~Observable(void) { }


/* Adds the observer to future updates */
void Observable::attach( Observer *observer ) {

}


/* Removes the observer from future updates */
void Observable::detach( Observer *observer ) {

}


/* Broadcasts updates to all observers */
void Observable::notify() {

}
