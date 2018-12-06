#pragma once
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
	// note:
	// could use one vector for better performance
	// but I doubt it's noticeable

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

private:

	ImplSubscriberArray _subscribers;
};


//// collision event information
//struct Collision
//	: Event<Collision>
//{
//	typedef int Entity; // (for this examples sake)
//
//	Collision(Entity E1, Entity E2)
//		: e1(E1),
//		e2(E2)
//	{
//	}
//
//	Entity e1;
//	Entity e2;
//};
//
//class MyReciever
//	: public Receiver<Collision>
//{
//public:
//
//	virtual void receive(const Collision& collision) /*override*/
//	{
//		std::cout << "Collision event occured with " << collision.e1 << " and " << collision.e2 << '\n';
//	}
//};

//int main(int argc, const char * argv[])
//{
//	MyReciever collisionReciever;
//	EventHandler handler;
//
//	// subscribe the reciever for events that fire Collision data
//	handler.subscribe<Collision>(&collisionReciever);
//
//	// make a collision object to send
//	Collision collision(5, 4);
//
//	// emit the event
//	handler.emit(collision);
//
//	return 0;
//}
