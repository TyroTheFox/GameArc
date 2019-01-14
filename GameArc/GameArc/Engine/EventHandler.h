#pragma once

#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <iostream>

class EventHandler {
public:
	using Func = std::function<void()>;
	using FuncInt = std::function<void(int x)>;
	using FuncVec2 = std::function<void(glm::vec2 vec)>;
	using FuncMessage = std::function<void(std::string message)>;
private:
	//Functors
	Func _func; //1
	FuncInt _funcInt; //2
	FuncVec2 _funcVec2; //3
	FuncMessage _funcMessage; //4
public:
	std::string name;
	int id;
	int counter;

	const int funcType;

	EventHandler() : id{ 0 }, funcType(1) {
	}

	EventHandler(const Func &func, const std::string& n) : _func{ func }, name(n), funcType(1){
		this->id = ++EventHandler::counter;
	}

	EventHandler(const FuncInt &func, const std::string& n) : _funcInt{ func }, name(n), funcType(2){
		this->id = ++EventHandler::counter;
	}

	EventHandler(const FuncMessage &func, const std::string& n) : _funcMessage{ func }, name(n), funcType(4){
		this->id = ++EventHandler::counter;
	}

	EventHandler(const FuncVec2 &func, const std::string& n) : _funcVec2{ func }, name(n), funcType(3){
		this->id = ++EventHandler::counter;
	}

	void operator()() {
		this->_func();
	}

	void operator()(int i) {
		this->_funcInt(i);
	}

	void operator()(std::string s) {
		this->_funcMessage(s);
	}

	void operator()(glm::vec2 vec) {
		this->_funcVec2(vec);
	}

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

	bool operator==(const EventHandler &del) {
		return this->id == del.id;
	}
	bool operator!=(nullptr_t) {
		return this->_func != nullptr || this->_funcInt != nullptr || this->_funcMessage != nullptr;
	}
};

class Event {
	//std::vector<std::unique_ptr<EventHandler>> handlers;
	std::map<std::string, std::unique_ptr<EventHandler>> handlers;
public:
	void notifyAllHandlers() {
		std::map<std::string, std::unique_ptr<EventHandler>>::iterator func = this->handlers.begin();
		for (; func != this->handlers.end(); ++func) {
			if (*func->second != nullptr && (*func->second).id != 0) {
				((*func->second))();
			}
		}
	}

	void notifyHandler(const std::string name) {
		(*handlers[name])();
	}

	void notifyHandlerWithMessage(const std::string name, const std::string message) {
		(*handlers[name])(message);
	}

	void notifyHandlerWithVec2(const std::string name, const glm::vec2 vec) {
		(*handlers[name])(vec);
	}

	void addHandler(const EventHandler &handler) {
		//this->handlers.push_back(unique_ptr<EventHandler>(new EventHandler{ handler }));
		this->handlers[handler.name] = std::unique_ptr<EventHandler>(new EventHandler{ handler });
	}

	void removeHandler(const EventHandler &handler) {
		std::map<std::string, std::unique_ptr<EventHandler>>::iterator to_remove = this->handlers.begin();
		for (; to_remove != this->handlers.end(); ++to_remove) {
			if ((*to_remove->second) == handler) {
				this->handlers.erase(to_remove);
				break;
			}
		}
	}

	void operator()(std::string name) {
		this->notifyHandler(name);
	}

	Event &operator+=(const EventHandler &handler) {
		this->addHandler(handler);

		return *this;
	}

	//Event &operator+=(const EventHandler::Func &handler) {
	//	this->addHandler(EventHandler{ handler });

	//	return *this;
	//}

	Event &operator-=(const EventHandler &handler) {
		this->removeHandler(handler);

		return *this;
	}
};