#pragma once
// EventCallback.h
class IEventCallback
{
public:
	virtual void operator() () = 0;
	// Other IEventCallback code
	virtual bool operator == (IEventCallback* other) = 0;
};

template <typename T, typename... Args>
class EventCallback : public IEventCallback
{
private:
	//Generic Class with Function Pointer to hold callback function

	void (T::*function)(Args... args);
	//Instance pointer
	T* instance;
public:

	EventCallback(T* instance, void (T::*function)(Args... args)) : instance(instance), function(function) {}
	//Fire the proxy function
	virtual void operator () () override { (instance->*function)(); }

	virtual bool operator == (IEventCallback* other) override
	{
		EventCallback* otherEventCallback = dynamic_cast<EventCallback*>(other);
		if (otherEventCallback == nullptr)
			return false;

		return 	(this->function == otherEventCallback->function) && (this->instance == otherEventCallback->instance);
	}
};