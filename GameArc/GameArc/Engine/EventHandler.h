#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <iostream>
#include <memory>

/**
* \class Event Handler
* \file Event Handler.h
* \author Kieran Clare
* \brief A wrapper for lambda functions used in event calling
*
* A wrapper for lambda functions used in event calling that can take multiple different inputs and handle necessary assigning functions
*/
class EventHandler {
public:
	using Func = std::function<void()>;///A standard void pointer function object
	using FuncInt = std::function<void(int x)>;///Void pointer function object that accepts an int object
	using FuncVec2 = std::function<void(glm::vec2 vec)>;///Void pointer function object that accepts a vector
	using FuncMessage = std::function<void(std::string message)>;///Void pointer function object that accepts a string object 
private:
	///Functors
	Func _func; //1
	FuncInt _funcInt; //2
	FuncVec2 _funcVec2; //3
	FuncMessage _funcMessage; //4
public:
	std::string name;///Event Handler name
	int id;///Unique Functor ID
	int counter;///Counter for number of stored functors so far for ID purposes

	const int funcType;///Type of Functor ID

	EventHandler() : id{ 0 }, funcType(1) {///Constructor
	}

	EventHandler(const Func &func, const std::string& n) : _func{ func }, name(n), funcType(1){///Void Functor Constructor
		this->id = ++EventHandler::counter;
	}

	EventHandler(const FuncInt &func, const std::string& n) : _funcInt{ func }, name(n), funcType(2){///Int Functor Constructor
		this->id = ++EventHandler::counter;
	}

	EventHandler(const FuncMessage &func, const std::string& n) : _funcMessage{ func }, name(n), funcType(4){///String Functor Constructor
		this->id = ++EventHandler::counter;
	}

	EventHandler(const FuncVec2 &func, const std::string& n) : _funcVec2{ func }, name(n), funcType(3){///Vector Functor Constructor
		this->id = ++EventHandler::counter;
	}

	void operator()() {///Fires Void Functor
		if(this->_func != nullptr)
		this->_func();
	}

	void operator()(int i) {///Fires Int Functor
		if (this->_funcInt != nullptr)
		this->_funcInt(i);
	}

	void operator()(std::string s) {///Fires String Functor
		if (this->_funcMessage != nullptr)
		this->_funcMessage(s);
	}

	void operator()(glm::vec2 vec) {///Fires Vector Functor
		if (this->_funcVec2 != nullptr)
		this->_funcVec2(vec);
	}

	void operator=(const EventHandler &func) {///Assignment Operator
		if (this->_func == nullptr) {
			this->_func = func;
			this->id = ++EventHandler::counter;
		}
		else {
			// Throw as exception or just log it out.
			std::cerr << "Nope!" << std::endl;
		}
	}

	bool operator==(const EventHandler &del) {///Equals Operator
		return this->id == del.id;
	}
	bool operator!=(nullptr_t) {///Does Not Equal Opperator
		return this->_func != nullptr || this->_funcInt != nullptr || this->_funcMessage != nullptr || this->_funcVec2 != nullptr;
	}
};

/**
* \class Event Handler
* \file Event Handler.h
* \author Kieran Clare
* \brief A global object that handles event calls via registered lambda functions
*
* Effectively, a list of wrapped lambda functions that allow for event calls to be made in any object that includes the global header, allowing for
* inter-object communication
*/
class Event {
	std::map<std::string, std::unique_ptr<EventHandler>> handlers;
public:
	///Fires Event Handler by name
	void notifyHandler(const std::string name) {
		if(handlers[name] != nullptr) (*handlers[name])();
	}
	///Fires Event by name with string message
	void notifyHandlerWithMessage(const std::string name, const std::string message) {
		if (handlers[name] != nullptr) (*handlers[name])(message);
	}
	///Fires Event by name with two-dimentional vector
	void notifyHandlerWithVec2(const std::string name, const glm::vec2 vec) {
		if (handlers[name] != nullptr) (*handlers[name])(vec);
	}
	///Fires Event by name with integer
	void notifyHandlerWithint(const std::string name, const int i) {
		if (handlers[name] != nullptr) (*handlers[name])(i);
	}
	///Adds new Event Handler
	void addHandler(const EventHandler &handler) {
		this->handlers[handler.name] = std::unique_ptr<EventHandler>(new EventHandler{ handler });
	}
	///Removes Event Handler from list
	void removeHandler(const EventHandler &handler) {
		std::map<std::string, std::unique_ptr<EventHandler>>::iterator to_remove = this->handlers.begin();
		for (; to_remove != this->handlers.end(); ++to_remove) {
			if ((*to_remove->second) == handler) {
				this->handlers.erase(to_remove);
				break;
			}
		}
	}
	///Removes event handler from list by name
	void removeHandler(const std::string name) {
		std::map<std::string, std::unique_ptr<EventHandler>>::iterator to_remove = this->handlers.find(name);
		this->handlers.erase(to_remove);
	}
	///Fires handler by name
	void operator()(std::string name) {
		this->notifyHandler(name);
	}
	///Adds new handler
	Event &operator+=(const EventHandler &handler) {
		this->addHandler(handler);

		return *this;
	}
	///Removes handler
	Event &operator-=(const EventHandler &handler) {
		this->removeHandler(handler);

		return *this;
	}
};