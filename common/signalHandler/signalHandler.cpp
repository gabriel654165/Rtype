
#include <signal.h>
#include <errno.h>
#include <iostream>
#include "signalHandler.hpp"

bool SignalHandler::mbGotExitSignal = false;

/**
* Default Contructor.
*/
SignalHandler::SignalHandler()
{
}
/**
* Destructor.
*/

SignalHandler::~SignalHandler()

{
}

/**
* Returns the bool flag indicating whether we received an exit signal
* Flag indicating shutdown of program
*/

bool SignalHandler::gotExitSignal()
{
    return mbGotExitSignal;
}

/**
* Sets the bool flag indicating whether we received an exit signal
*/
void SignalHandler::setExitSignal(bool _bExitSignal)
{
    mbGotExitSignal = _bExitSignal;
}

/**
* Sets exit signal to true.
* @param[in] _ignored Not used but required by function prototype
*                     to match required handler.

*/

void SignalHandler::exitSignalHandler([[maybe_unused]]int _ignored)

{
    std::cout << "got signal handler" << "\n";
    mbGotExitSignal = true;
}

void SignalHandler::setupSignalHandlers()
{
    signal((int)SIGINT, SignalHandler::exitSignalHandler);
}
