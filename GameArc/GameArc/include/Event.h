#pragma once
#include "EventCallback.h"

#include <vector>

using namespace std;
//Event.h
class Event
{
private:
	typedef std::vector<IEventCallback*> CallbackArray;
	CallbackArray actions;
public:
	Event();
	~Event();

	void addListener(IEventCallback* action);
	void removeListener(IEventCallback* action);
	void fire();
};