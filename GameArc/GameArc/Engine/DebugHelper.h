#pragma once

#include <string>
#include <vector>
#include "IEngineCore.h"
#include "TextWriter.h"
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include "global.h"

///Forward Declared Objects
extern Event* keyEvent;
class IEngineCore;
class TextWriter;

/**
* \class Text Parser
* \file DebugHelper.h
* \brief A text parser class that takes in strings and interprets them into function fires
*
* A class that splits up string commands into tokens and uses them to find the correct lambda function to fire, 
* complete with additional arguments to pass in
*/
class TextParser {
public:
	using InterpFunc = std::function<void(std::vector<std::string>)>;///Void Pointer Functor Object that takes in a vector of strings for extra variables
private:
	std::map<std::string, InterpFunc> parsedTokens;///List of functors sorted by function name token
public:
	TextParser() {///Constructor
	}
	void AddToken(std::string token, InterpFunc f) {///Add new token and function
		parsedTokens[token] = f;
	}
	bool ParseToken(std::string command) {///Parse string command to create a function action from the command
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
			parsedTokens[token](argTokens);
		}
		else {
			return false;
		}
		return true;
	}
};

/**
* \class Debug Helper
* \file DebugHelper.h
* \brief A debug rendering and console class that contains useful debugging tools
*
* A debug class that contains the debug console and functions needed to output to the debug console
*/
class DebugHelper {
private:
	TextWriter* textWriter; ///2D text rendering object
	float consoleX = 10.0f, consoleY = 10.0f, spacing = 20.0f; ///Console positioning variables
	std::vector<std::string> consoleLines;///Lines of console to render
	int lineLimit = 5;///Limit to console lines to render
	float timePassed = 0.0f, removeConsoleLine = 5.0f;///Timing variables used to slowly remove console lines over time

	std::string consoleInput = "";///Current console line contents
	TextParser* textParser;///Text parser object for interpreting the console line
	bool caps = false;///Capital leter flag
public:
	bool displayConsole = false;///Display console flag
	DebugHelper(IEngineCore* enginePtr);///Constructor
	void ToggleDisplayConsole();///Flips display console flag on call
	void HandleInputLine(int i);///Appends char inputs from Input Handler to consoleInput
	void DeleteLastChar();///Removes last character from consoleInput
	void ProcessInputLine();///Processes and interprets the consoleInput line for further use
	void update(float dt);///Called every update tick, updates all console variables (even when not on screen)
	void render();///Called every render call, draws the console to the screen
	void WriteToConsole(std::string message);///Writes text to the console for rendering
	void AddConsoleCommand(std::string commandName, TextParser::InterpFunc f);///Adds a new console command for the TextParser to record and recognise when called
};