#include "../include/Event.h"

Event::Event() { }
Event::~Event() { }

void Event::addListener(IEventCallback* action)
{
	CallbackArray::iterator position = find(actions.begin(), actions.end(), action);

	if (position != actions.end())
	{
		//Debug::LogWarning("Action existed in delegate list.");
		return;
	}

	actions.push_back(action);
}

void Event::removeListener(IEventCallback* action)
{
	CallbackArray::iterator position = find(actions.begin(), actions.end(), action);

	if (position == actions.end())
	{
		return;
	}

	actions.erase(position);
}

void Event::fire()
{
	for (IEventCallback* action : actions)
	{
		(*action)();
	}
}