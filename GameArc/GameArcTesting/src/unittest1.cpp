#define USE_GLFW 1
#ifdef USE_GLFW
#include <Engine/GLFW_EngineCore.h>
#endif
#include <Engine/DebugHelper.h>
#include <Engine/Game.h>
#include <Engine/Model.h>
#include <Engine/global.h>

#include "CppUnitTest.h"

extern Event* keyEvent;
extern InputHandler* inputHandler;
extern ModelHandler* modelHandler;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameArcTesting
{

	TEST_CLASS(EventTesting)
	{
	/*public:
		int windowWidth = 800, windowHeight = 600;
		bool HasSent = false;
		std::string recievedMessage;
		int recievedInt;
		float recievedFloat, recievedFloat2, recievedFloat3;

		TEST_METHOD(InputLoadingTest)
		{
			inputHandler = new InputHandler();
			keyEvent = new Event();
			Assert::IsNotNull(inputHandler);
			Assert::IsTrue(inputHandler->loadFromJSON("H:/Documents/GitHub/GameArc/GameArc/GameArcTesting/assets/inputs.json"));
			Assert::IsFalse(inputHandler->loadFromJSON("assets"));
		}
		
		TEST_METHOD(InputCommandTest)
		{
			inputHandler = new InputHandler();
			keyEvent = new Event();
			Assert::IsNotNull(inputHandler);
			Assert::IsTrue(inputHandler->loadFromJSON("H:/Documents/GitHub/GameArc/GameArc/GameArcTesting/assets/inputs.json"));

			for (auto input : inputHandler->m_controlMapping) {
				InputCommand* key = inputHandler->m_controlMapping[input.first];
				Assert::IsNotNull(key);
				if (key->eventType == "KeyInputFuncEvent") {
					KeyInputFuncEvent* e = (KeyInputFuncEvent*)key;
					EventHandler::Func test = [this] { this->HasSent = true; };
					EventHandler testListener(test, e->eventName);
					keyEvent->addHandler(testListener);
					key->execute();
					Assert::IsTrue(HasSent);
					HasSent = false;
				}
				if (key->eventType == "KeyInputEvent") {
					KeyInputEvent* e = (KeyInputEvent*)key;
					EventHandler::FuncMessage test = [this](std::string message) { this->HasSent = true; this->recievedMessage = message; };
					EventHandler testListener(test, e->eventName);
					keyEvent->addHandler(testListener);
					key->execute();
					Assert::IsTrue(HasSent);
					Assert::AreEqual(e->message, recievedMessage);
					HasSent = false;
					recievedMessage = "";
				}
				if (key->eventType == "KeyInputIntEvent") {
					KeyInputIntEvent* e = (KeyInputIntEvent*)key;
					EventHandler::FuncInt test = [this](int message) { this->HasSent = true; this->recievedInt = message; };
					EventHandler testListener(test, e->eventName);
					keyEvent->addHandler(testListener);
					key->execute();
					Assert::IsTrue(HasSent);
					Assert::AreEqual(e->message, recievedInt);
					HasSent = false;
					recievedInt = 0;
				}
			}
		}

		TEST_METHOD(DebugInputCommandTest)
		{
			inputHandler = new InputHandler();
			keyEvent = new Event();
			Assert::IsNotNull(inputHandler);
			Assert::IsTrue(inputHandler->loadFromJSON("H:/Documents/GitHub/GameArc/GameArc/GameArcTesting/assets/inputs.json"));

			vector<int> debugkeys = { (int)'`', 257, 259, 344 };

			EventHandler::Func displayMessage = [this] { this->HasSent = true; };
			EventHandler displayListener(displayMessage, "ToggleDebugConsole");
			keyEvent->addHandler(displayListener);

			EventHandler::Func consoleEnterHit = [this] { this->HasSent = true; };
			EventHandler consoleEnterHitListener(consoleEnterHit, "DebugEnterHit");
			keyEvent->addHandler(consoleEnterHitListener);

			EventHandler::Func consoleBackspaceHit = [this] { this->HasSent = true; };
			EventHandler consoleBackspaceHitListener(consoleBackspaceHit, "DebugBackspaceHit");
			keyEvent->addHandler(consoleBackspaceHitListener);

			EventHandler::FuncInt consoleShiftHit = [this](int i) { this->HasSent = true; };
			EventHandler consoleShiftHitListener(consoleShiftHit, "DebugShiftHit");
			keyEvent->addHandler(consoleShiftHitListener);

			for (auto input : debugkeys) {
				InputCommand* key = inputHandler->m_debugMapping[input];
				Assert::IsNotNull(key);
				key->execute();
				Assert::IsTrue(HasSent);
				HasSent = false;
			}
		}

		TEST_METHOD(DebugInputTest1) {
			inputHandler = new InputHandler();
			keyEvent = new Event();

			TextParser* textParser = new TextParser();

			std::string sentMessage = "TESTING";

			textParser->AddToken("parserTest1", TextParser::InterpFunc([this](std::vector<std::string> s) {
				if (s.size() <= 0) { this->recievedInt = 1; return; }
				this->recievedMessage = s.at(0);
			}));

			textParser->ParseToken("parserTest1 " + sentMessage);

			Assert::IsTrue(recievedInt == 0);
			Assert::AreEqual(sentMessage, recievedMessage);
			recievedMessage = "";
			recievedInt = 0;
		}

		TEST_METHOD(DebugInputTest2) {
			inputHandler = new InputHandler();
			keyEvent = new Event();

			TextParser* textParser = new TextParser();

			float sentMessage = 7.39234f;

			textParser->AddToken("parserTest2", TextParser::InterpFunc([this](std::vector<std::string> s) {
				if (s.size() <= 0) { this->recievedInt = 1; return; }
				this->recievedFloat = std::stof(s.at(0));
			}));

			textParser->ParseToken("parserTest2 " + std::to_string(sentMessage));

			Assert::IsTrue(recievedInt == 0);
			Assert::AreEqual(sentMessage, recievedFloat);
			recievedFloat = 0.0f;
			recievedInt = 0;
		}

		TEST_METHOD(DebugInputTest3) {
			inputHandler = new InputHandler();
			keyEvent = new Event();

			TextParser* textParser = new TextParser();

			float sentMessage = 7.39234f;
			float sentMessage2 = 1000.0f;
			float sentMessage3 = 2.4f;

			textParser->AddToken("parserTest3", TextParser::InterpFunc([this](std::vector<std::string> s) {
				if (s.size() <= 0) { this->recievedInt = 1; return; }
				this->recievedFloat = std::stof(s.at(0));
				if (s.size() > 1)
				this->recievedFloat2 = std::stof(s.at(1));
				if (s.size() > 2)
				this->recievedFloat3 = std::stof(s.at(2));
			}));

			textParser->ParseToken("parserTest3 " + std::to_string(sentMessage) + " " + std::to_string(sentMessage2) + " " + std::to_string(sentMessage3));

			Assert::IsTrue(recievedInt == 0);
			Assert::AreEqual(sentMessage, recievedFloat);
			Assert::AreEqual(sentMessage2, recievedFloat2);
			Assert::AreEqual(sentMessage3, recievedFloat3);
			recievedFloat = 0.0f;
			recievedFloat2 = 0.0f;
			recievedFloat3 = 0.0f;
			recievedInt = 0;
		}


		TEST_METHOD(DebugInputTest4) {
			inputHandler = new InputHandler();
			keyEvent = new Event();

			TextParser* textParser = new TextParser();

			float sentMessage = 7.39234f;

			textParser->AddToken("parserTest4", TextParser::InterpFunc([this](std::vector<std::string> s) {
				if (s.size() <= 0) { this->recievedInt = 1; return; }
				this->recievedFloat = std::stof(s.at(0));
				if (s.size() > 1)
					this->recievedFloat2 = std::stof(s.at(1));
				if (s.size() > 2)
					this->recievedFloat3 = std::stof(s.at(2));
			}));

			textParser->ParseToken("parserTest4 " + std::to_string(sentMessage));

			Assert::IsTrue(recievedInt == 0);
			Assert::AreEqual(sentMessage, recievedFloat);
			recievedFloat = 0.0f;
			recievedInt = 0;
		}*/
	};
}