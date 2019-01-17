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

extern Event* keyEvent;
class IEngineCore;
class TextWriter;

class TextParser {
public:
	using InterpFunc = std::function<void(std::vector<std::string>)>;
private:
	std::map<std::string, InterpFunc> parsedTokens;
public:
	TextParser() {
	}
	void AddToken(std::string token, InterpFunc f) {
		parsedTokens[token] = f;
	}
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
			parsedTokens[token](argTokens);
		}
		else {
			return false;
		}
		return true;
	}
};

class DebugHelper {
private:
	TextWriter* textWriter; 
	float consoleX = 10.0f, consoleY = 10.0f, spacing = 20.0f;
	std::vector<std::string> consoleLines;
	int lineLimit = 5;
	float timePassed = 0.0f, removeConsoleLine = 5.0f;

	std::string consoleInput = "";
	TextParser* textParser;
public:
	bool displayConsole = false;
	DebugHelper(IEngineCore* enginePtr);
	void ToggleDisplayConsole();
	void HandleInputLine(int i);
	void DeleteLastChar();
	void ProcessInputLine();
	void update(float dt);
	void render();
	void WriteToConsole(std::string message);
	void AddConsoleCommand(std::string commandName, TextParser::InterpFunc f);
};