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
	///A standard void pointer function object
	using Func = std::function<void()>;
	///Void pointer function object that accepts an int object
	using FuncInt = std::function<void(int x)>;
	///Void pointer function object that accepts a vector
	using FuncVec2 = std::function<void(glm::vec2 vec)>;
	///Void pointer function object that accepts a string object 
	using FuncMessage = std::function<void(std::string message)>;
private:
	///Functors
	Func _func; //1
	FuncInt _funcInt; //2
	FuncVec2 _funcVec2; //3
	FuncMessage _funcMessage; //4
public:
	///Event Handler name
	std::string name;
	///Unique Functor ID
	int id;
	///Counter for number of stored functors so far for ID purposes
	int counter;
	///Type of Functor ID
	const int funcType;
	///Constructor
	EventHandler() : id{ 0 }, funcType(1) {
	}
	///Void Functor Constructor
	EventHandler(const Func &func, const std::string& n) : _func{ func }, name(n), funcType(1){
		this->id = ++EventHandler::counter;
	}
	///Int Functor Constructor
	EventHandler(const FuncInt &func, const std::string& n) : _funcInt{ func }, name(n), funcType(2){
		this->id = ++EventHandler::counter;
	}
	///String Functor Constructor
	EventHandler(const FuncMessage &func, const std::string& n) : _funcMessage{ func }, name(n), funcType(4){
		this->id = ++EventHandler::counter;
	}
	///Vector Functor Constructor
	EventHandler(const FuncVec2 &func, const std::string& n) : _funcVec2{ func }, name(n), funcType(3){
		this->id = ++EventHandler::counter;
	}
	///Fires Void Functor
	void operator()() {
		if(this->_func != nullptr)
		this->_func();
	}
	///Fires Int Functor
	void operator()(int i) {
		if (this->_funcInt != nullptr)
		this->_funcInt(i);
	}
	///Fires String Functor
	void operator()(std::string s) {
		if (this->_funcMessage != nullptr)
		this->_funcMessage(s);
	}
	///Fires Vector Functor
	void operator()(glm::vec2 vec) {
		if (this->_funcVec2 != nullptr)
		this->_funcVec2(vec);
	}
	///Assignment Operator
	void operator=(const EventHandler &func) {
		if (this->_func == nullptr) {
			this->_func = func;
			this->id = ++EventHandler::counter;
		}
		else {
			// Throw as exception or just log it out.
			std::cerr << "Nope!" << std::endl;
		}
	}
	///Equals Operator
	bool operator==(const EventHandler &del) {
		return this->id == del.id;
	}
	///Does Not Equal Opperator
	bool operator!=(nullptr_t) {
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