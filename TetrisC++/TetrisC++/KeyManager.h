#pragma once
#include <vector>
#include <map>
#include <Windows.h>

enum class HOW_KEY_WORK
{
	PressToWork,
	OperatesWhenPressedWhenNotPressed
};

struct KeyData
{
	HOW_KEY_WORK howKeyWork;
	bool isKeyDown;
};

class KeyManager
{
public:
	void Init();
	void InitKeyData(std::vector<int> vKeys, HOW_KEY_WORK howKeyWork);
	bool KeyDown(int vKey);
	void Update();
private:
	std::map<int, KeyData> keyMap;
	float totalTime = 0.0f, lastInputTime = 0.0f, inputDelay = 0.1f, keyTimeSpeed = 0.01f;
};

