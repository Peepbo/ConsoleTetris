#pragma once
#include "defineHeader.h"
#include <string>
#include <unordered_map>

class RankManager;

class Draw
{
public:
	void SetWindowSize(const int& width,const int& height);

	void ClearScreen();
	void ShowConsoleCursor(const bool& showFlag);

	void NextAndSaveBlockDisplay(const BlockData& nextBlockData, const BlockData& saveBlockData);
	void Display(const vvb& matrix, const float& time, const int& score, const BlockData& nextBlockData, const BlockData& saveBlockData, 
		const std::unordered_map <Point, int, std::hash<Point>>& landingMap);
	void InputName(std::string& nickName);
	void RankDisplay(const std::vector<std::string>& data);
private:
	std::string space, margin;
};