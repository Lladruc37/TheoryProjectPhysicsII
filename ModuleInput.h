#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"
#include "Globals.h"

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	ModuleInput(Application* app, bool startEnabled = true);
	~ModuleInput();

	bool Init();
	UpdateStatus PreUpdate();
	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	int GetMouseX() const
	{
		return mouseX;
	}

	int GetMouseY() const
	{
		return mouseY;
	}

private:
	KeyState* keyboard;
	int mouseX;
	int mouseY;
};

#endif // !__MODULE_INPUT_H__