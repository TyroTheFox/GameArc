#pragma once
#include "ModelHandler.h"
#include "InputHandler.h"
#include "EventHandler.h"

class ModelHandler;
extern __declspec(selectany) ModelHandler* modelHandler;

extern __declspec(selectany) InputHandler* inputHandler;

extern __declspec(selectany) Event* keyEvent;