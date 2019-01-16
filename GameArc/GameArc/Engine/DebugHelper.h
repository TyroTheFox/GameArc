#pragma once
#include <string>
#include <vector>
#include "IEngineCore.h"
#include "TextWriter.h"
#include "global.h"

extern Event* keyEvent;
class IEngineCore;
class TextWriter;

class DebugHelper {
private:
	TextWriter* textWriter; 
	float consoleX = 10.0f, consoleY = 10.0f, spacing = 20.0f;
	std::vector<std::string> consoleLines;
	int lineLimit = 5;
	float timePassed = 0.0f, removeConsoleLine = 5.0f;

	std::string consoleInput;
public:
	bool displayConsole = false;
	DebugHelper(IEngineCore* enginePtr);
	void ToggleDisplayConsole();
	void HandleInputLine(int i);
	void ProcessInputLine();
	void update(float dt);
	void render();
	void WriteToConsole(std::string message);
};