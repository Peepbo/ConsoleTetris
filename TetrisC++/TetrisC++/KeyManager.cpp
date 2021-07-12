#include "KeyManager.h"

void KeyManager::Init()
{
	InitKeyData({ VK_LEFT, VK_RIGHT, VK_DOWN }, HOW_KEY_WORK::PressToWork);

	InitKeyData({ VK_UP, 0x58/*X*/, 0x5A/*Z*/,
		VK_OEM_COMMA/*+*/,VK_OEM_MINUS/*-*/,
		VK_SPACE }, HOW_KEY_WORK::OperatesWhenPressedWhenNotPressed);
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

		if (keyMap[vKey].isKeyDown && !isDown)
			keyMap[vKey].isKeyDown = false;

		if (!keyMap[vKey].isKeyDown && isDown)
		{
			keyMap[vKey].isKeyDown = true;
			return true;
		}
	}

	return false;
}

void KeyManager::Update()
{
	totalTime += keyTimeSpeed;
}
