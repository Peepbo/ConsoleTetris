#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct RankData
{
	int score;
	std::string nickName;
};

class RankManager
{
public:
	//function
	bool CheckScore(const int& score);
	void RecordScore(const std::string& fileName, const std::string& nickName, const int& score);
	std::vector<std::string> GetRankData(const std::string& fileName);
};

