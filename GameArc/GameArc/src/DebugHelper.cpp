#include "../Engine/DebugHelper.h"

DebugHelper::DebugHelper(IEngineCore* enginePtr)
{
	textWriter = new TextWriter();
	textWriter->m_engineInterfacePtr = enginePtr;

	textParser = new TextParser();

	EventHandler::Func displayMessage = [this] { this->ToggleDisplayConsole(); };
	EventHandler displayListener(displayMessage, "ToggleDebugConsole");
	keyEvent->addHandler(displayListener);

	EventHandler::FuncInt consoleInputMessage = [this](int i) { this->HandleInputLine(i); };
	EventHandler consoleInputMessageListener(consoleInputMessage, "DebugConsoleInput");
	keyEvent->addHandler(consoleInputMessageListener);

	EventHandler::Func consoleEnterHit = [this] { this->ProcessInputLine(); };
	EventHandler consoleEnterHitListener(consoleEnterHit, "DebugEnterHit");
	keyEvent->addHandler(consoleEnterHitListener);

	EventHandler::Func consoleBackspaceHit = [this] { this->DeleteLastChar(); };
	EventHandler consoleBackspaceHitListener(consoleBackspaceHit, "DebugBackspaceHit");
	keyEvent->addHandler(consoleBackspaceHitListener);
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
	consoleInput += c;
}

void DebugHelper::DeleteLastChar() { if (consoleInput.size() > 0) {consoleInput.pop_back(); }
}

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

void DebugHelper::AddConsoleCommand(std::string commandName, TextParser::InterpFunc f) {
	textParser->AddToken(commandName, f);
}