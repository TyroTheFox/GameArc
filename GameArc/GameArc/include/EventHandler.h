#pragma once
//https://ideone.com/GIa77s
#include <iostream>
#include <vector>

class BaseReceiever
{
protected:

	static size_t nextId;
};

size_t BaseReceiever::nextId = 0;

// if you inherit from this class,
// you are required to implement
// void recieve(const TEventData& eventData)
// where TEventData is an Event (see further down)
template <typename TEventData>
class Receiver
	: public BaseReceiever
{
public:

	virtual void receive(const TEventData&) = 0;

	static size_t GetId()
	{
		return _id;
	}

private:

	// used for internal storage
	// identification
	static size_t _id;
};

template<typename T>
size_t Receiver<T>::_id = BaseReceiever::nextId++;

struct BaseEvent
{
protected:

	static size_t nextId;
};

size_t BaseEvent::nextId = 0;

template <typename T>
struct Event
	: public BaseEvent
{
	static size_t GetId()
	{
		return _id;
	}

private:

	static size_t _id;
};

template <typename T>
size_t Event<T>::_id = BaseEvent::nextId++;

class EventHandler
{
	typedef std::vector<BaseReceiever*> ReceiverArray;
	typedef std::vector<ReceiverArray> ImplSubscriberArray;

public:

	template <typename TEvent>
	void emit(const TEvent& event)
	{
		ReceiverArray& receivers = _subscribers[TEvent::GetId()];
		for (ReceiverArray::iterator i = receivers.begin(); i != receivers.end(); ++i)
		{
			static_cast<Receiver<TEvent>&>(*(*i)).receive(event);
		}
	}

	template <typename TEvent, typename TReciever>
	void subscribe(TReciever* receiver)
	{
		size_t eventId = TEvent::GetId();
		size_t receiverId = TReciever::GetId();

		if (_subscribers.size() <= eventId)
		{
			_subscribers.resize(eventId + 1);
		}

		ReceiverArray& receivers = _subscribers[eventId];

		if (receivers.size() <= receiverId)
		{
			receivers.resize(receiverId + 1);
		}

		receivers[receiverId] = receiver;
	}

	template <typename TEvent, typename TReciever>
	void unsubscribe(TReciever* reciever)
	{
		_subscribers[TEvent::GetId()][TReciever::GetId()] = NULL;
	}

	static EventHandler * instance()
	{
		if (!s_instance)
			s_instance = new EventHandler;
		return s_instance;
	}

private:
	static EventHandler *s_instance;
	EventHandler();
	ImplSubscriberArray _subscribers;
};
EventHandler * EventHandler::s_instance = 0;
struct KeyEvent : Event<KeyEvent>
{
	KeyEvent(int E1, bool E2) : e1(E1),	e2(E2)
	{}

	int e1;
	bool e2;
};