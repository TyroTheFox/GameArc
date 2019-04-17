#include "../Engine/DebugHelper.h"

DebugHelper::DebugHelper(IEngineCore* enginePtr)
{
	textWriter = new TextWriter(enginePtr);

	textParser = new TextParser();

	keyEvent->subscribeToEvent("ToggleDebugConsole", [this] { this->ToggleDisplayConsole(); });
	keyEvent->subscribeToEvent("DebugConsoleInput", [this](int i) { this->HandleInputLine(i); });
	keyEvent->subscribeToEvent("DebugEnterHit", [this] { this->ProcessInputLine(); });
	keyEvent->subscribeToEvent("DebugBackspaceHit", [this] { this->DeleteLastChar(); });
	keyEvent->subscribeToEvent("DebugShiftHit", [this](int i) { caps = i; });
}

void DebugHelper::update(float dt)
{
	if (consoleLines.size() > 0) {
		if (consoleLines.size() > lineLimit) {
			consoleLines.erase(consoleLines.begin());
		}

		timePassed += dt;
		if (timePassed > removeConsoleLine) {
			consoleLines.erase(consoleLines.begin());
			timePassed = 0.0f;
		}
	}
}

void DebugHelper::ToggleDisplayConsole() {
	displayConsole = !displayConsole;
	if (displayConsole) { WriteToConsole("Debug Console ON"); }
}

void DebugHelper::HandleInputLine(int i) {
	char c = i;
	if (caps) {
		c = toupper(c);
	}
	consoleInput += c;
}

void DebugHelper::DeleteLastChar() { if (consoleInput.size() > 0) { consoleInput.pop_back(); } }

void DebugHelper::ProcessInputLine() {
	if (textParser->ParseToken(consoleInput)) {
		WriteToConsole(consoleInput);
	}
	else {
		WriteToConsole("Command '" + consoleInput + "' Not Recognised");
	}
	consoleInput = "";
}

void DebugHelper::render()
{
	if (displayConsole) {

		textWriter->DrawNormalText("> " + consoleInput + "_", consoleX, consoleY + (spacing * (lineLimit + 2)), 0.5f);

		int i = 0;
		for (auto line : consoleLines) {
			//Normal
			if (line.second == 0) {
				textWriter->DrawNormalText(line.first, consoleX, consoleY + (spacing * i), 0.5f);
			}
			//Error
			if (line.second == 1) {
				textWriter->DrawErrorText(line.first, consoleX, consoleY + (spacing * i), 0.5f);
			}
			i++;
		}
	}
}

void DebugHelper::WriteToConsole(std::string message)
{
	consoleLines.insert(std::make_pair(message, 0));
}

void DebugHelper::WriteErrorToConsole(std::string message)
{
	consoleLines.insert(std::make_pair(message, 1));
}

void DebugHelper::AddConsoleCommand(std::string commandName, TextParser::InterpFunc f) {
	textParser->AddToken(commandName, f);
}

void DebugHelper::RunCommand(std::string commandName)
{
	if (textParser->ParseToken(commandName)) {
		WriteToConsole(commandName);
	}
	else {
		WriteToConsole("Command '" + commandName + "' Not Recognised");
	}
}
