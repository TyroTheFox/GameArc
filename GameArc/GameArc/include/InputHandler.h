#ifndef _RDB_EVENT_SYSTEM_H_
#define _RDB_EVENT_SYSTEM_H_
#pragma once
//http://tongtunggiang.com/blog/2017/08/22/implement-simple-event-delegate-system/
#include "Event.h"

#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
#include <map>

using std::vector;

class InputHandler
{
private:
	static InputHandler *s_instance;
	std::map<std::string, Event*> inputEvents;
	InputHandler() {}
public:
	void registerAction(const std::string& actionName, IEventCallback* action)
	{
		if (inputEvents.count(actionName) > 1) {
			//Debug::LogError("There is no action named " + actionName);
			return;
		}
		inputEvents[actionName]->addListener(action);
	}

	bool callAction(const std::string& actionName) {

		if (inputEvents[actionName] != nullptr)
		{
			inputEvents[actionName]->fire();
			return true;
		}
		return false;
	}

	static InputHandler * instance()
	{
		if (!s_instance)
			s_instance = new InputHandler;
		return s_instance;
	}
}

#endif // _RDB_EVENT_SYSTEM_H_