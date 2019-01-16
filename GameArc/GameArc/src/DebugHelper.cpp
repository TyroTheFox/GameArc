#include "../Engine/DebugHelper.h"

DebugHelper::DebugHelper(IEngineCore* enginePtr)
{
	textWriter = new TextWriter();
	textWriter->m_engineInterfacePtr = enginePtr;

	EventHandler::Func displayMessage = [this] { this->ToggleDisplayConsole(); };
	EventHandler displayListener(displayMessage, "ToggleDebugConsole");
	keyEvent->addHandler(displayListener);
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

void DebugHelper::render()
{
	if (displayConsole) {
		int i = 0;
		for (std::string line : consoleLines) {
			textWriter->DrawNormalText(line, consoleX, consoleY + (spacing * i), 0.5f);
			i++;
		}
	}
}

void DebugHelper::WriteToConsole(std::string message)
{
	consoleLines.push_back(message);
}
