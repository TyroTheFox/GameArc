#pragma once
//http://tongtunggiang.com/blog/2017/08/22/implement-simple-event-delegate-system/

#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>

#include "GameObject.h"
#include "Component.h"
#include "Event.h"

class InputHandler
{
private:
	static InputHandler *s_instance;
	std::map<std::string, Event<GameObject*>> inputEvents;
	InputHandler() {}
public:
	void registerAction(const std::string& actionName, Event<GameObject*> action)
	{
		//if (inputEvents.count(actionName) > 1) {
			//Debug::LogError("There is no action named " + actionName);
		//	return;
		//}
		inputEvents[actionName] = action;
	}

	bool callAction(const std::string& actionName) {

		if (inputEvents.count(actionName) < 0)
		{
			inputEvents[actionName].Call();
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