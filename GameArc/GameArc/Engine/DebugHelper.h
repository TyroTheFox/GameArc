#pragma once
#include "global.h"
#include <string>
#include <vector>
#include "TextWriter.h"

extern Event* keyEvent;

class DebugHelper {
private:
	TextWriter* textWriter; 
	float consoleX = 10.0f, consoleY = 10.0f, spacing = 20.0f;
	std::vector<std::string> consoleLines;
	int lineLimit = 5;
	float timePassed = 0.0f, removeConsoleLine = 5.0f;
	bool displayConsole = false;
public:
	DebugHelper();
	void setWindowSize(float x, float y);
	void update(float dt);
	void ToggleDisplayConsole();
	void render();
	void WriteToConsole(std::string message);
};