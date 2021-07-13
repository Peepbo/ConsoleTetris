#include "KeyManager.h"

void KeyManager::Init()
{
	InitKeyData({ VK_LEFT, VK_RIGHT, VK_DOWN }, HOW_KEY_WORK::PressToWork);

	InitKeyData({ VK_UP, 0x58/*X*/, 0x5A/*Z*/,
		VK_ADD/*+*/,VK_SUBTRACT/*-*/,
		VK_SPACE,VK_SHIFT }, HOW_KEY_WORK::OperatesWhenPressedWhenNotPressed);
}

void KeyManager::InitKeyData(std::vector<int> vKeys, HOW_KEY_WORK howKeyWork)
{
	for (const int& vkey : vKeys)
	{
		keyMap[vkey] = { howKeyWork,false };
	}
}

bool KeyManager::KeyDown(int vKey)
{
	bool isDown = GetAsyncKeyState(vKey);

	switch (keyMap[vKey].howKeyWork)
	{
	case HOW_KEY_WORK::PressToWork:

		if (!isDown || totalTime - lastInputTime < inputDelay)
			return false;
		else
		{
			lastInputTime = totalTime;
			return true;
		}
	case HOW_KEY_WORK::OperatesWhenPressedWhenNotPressed:

		bool& isKeyDown = keyMap[vKey].isKeyDown;

		if (isKeyDown && !isDown)
			isKeyDown = false;

		if (!isKeyDown && isDown)
		{
			isKeyDown = true;
			return true;
		}
	}

	return false;
}

void KeyManager::Update()
{
	totalTime += keyTimeSpeed;
}
