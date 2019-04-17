#pragma once

#include <string>
#include <vector>
#include "IEngineCore.h"
#include "TextWriter.h"
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <functional>
#include "global.h"

///Forward Declared Objects
extern Event* keyEvent;
///Forward Declared Objects
class IEngineCore;
///Forward Declared Objects
class TextWriter;

/** \class TextParser
\file DebugHelper.h
\brief A text parser class that takes in strings and interprets them into function fires

A class that splits up string commands into tokens and uses them to find the correct lambda function to fire, 
complete with additional arguments to pass in
*/
class TextParser {
public:
	///Void Pointer Functor Object that takes in a vector of strings for extra variables
	using InterpFunc = std::function<void(std::vector<std::string>)>;
private:
	///List of functors sorted by function name token
	std::multimap<std::string, InterpFunc> parsedTokens;
public:
	///Constructor
	TextParser() {
	}
	///Add new token and function
	void AddToken(std::string token, InterpFunc f) {
		parsedTokens.insert(std::make_pair(token, f));
	}
	///Parse string command to create a function action from the command
	bool ParseToken(std::string command) {
		std::stringstream ss(command);
		std::string token, argToken;
		ss >> token;
		std::vector<std::string> argTokens;
		while (!ss.eof()) {
			//std::getline(ss, argToken);
			ss >> argToken;
			argTokens.push_back(argToken);
		}
		
		if (parsedTokens.count(token) > 0) {
			auto search = parsedTokens.equal_range(token);
			for (auto i = search.first; i != search.second; ++i){
				(i->second)(argTokens);
				//parsedTokens[token](argTokens);
			}
		}
		else {
			return false;
		}
		return true;
	}
};

/** \class DebugHelper
\file DebugHelper.h
\brief A debug rendering and console class that contains useful debugging tools

A debug class that contains the debug console and functions needed to output to the debug console
*/
class DebugHelper {
private:
	///2D text rendering object
	TextWriter* textWriter;  
	///Console positioning variables
	float consoleX = 10.0f, consoleY = 10.0f, spacing = 20.0f; 
	///Lines of console to render
	std::map<std::string, int> consoleLines;
	///Limit to console lines to render
	int lineLimit = 5;
	///Timing variables used to slowly remove console lines over time
	float timePassed = 0.0f, removeConsoleLine = 5.0f;
	///Current console line contents
	std::string consoleInput = "";
	///Text parser object for interpreting the console line
	TextParser* textParser;
	///Capital leter flag
	bool caps = false;
public:
	///Display console flag
	bool displayConsole = false;
	///Constructor
	DebugHelper(IEngineCore* enginePtr);
	///Flips display console flag on call
	void ToggleDisplayConsole();
	///Appends char inputs from Input Handler to consoleInput
	void HandleInputLine(int i);
	///Removes last character from consoleInput
	void DeleteLastChar();
	///Processes and interprets the consoleInput line for further use
	void ProcessInputLine();
	///Called every update tick, updates all console variables (even when not on screen)
	void update(float dt);
	///Called every render call, draws the console to the screen
	void render();
	///Writes text to the console for rendering
	void WriteToConsole(std::string message);
	///Writes error text to the console for rendering
	void WriteErrorToConsole(std::string message);
	///Adds a new console command for the TextParser to record and recognise when called
	void AddConsoleCommand(std::string commandName, TextParser::InterpFunc f);

	void RunCommand(std::string commandName);
};