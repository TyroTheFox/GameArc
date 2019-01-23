#pragma once
/** \class Global Variables
* \file global.h
* \brief A header file that contains global external objects for the Model Handler, the Input Handler and the Key Event objects
*
* A header file that contains global external objects that can then be used by any class in the project. These are the Model Handler, The Input Handler and the Key Event. 
* These are external objects that are accessable by all classes but must be initialised before use. 
*/
#include "EventHandler.h"
///External Key Event, compiler is directed to pick only one of any given instance of object
extern __declspec(selectany) Event* keyEvent;