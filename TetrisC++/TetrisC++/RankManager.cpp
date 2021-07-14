#include "RankManager.h"

bool RankManager::CheckScore(const int& score)
{
	return score == 0? false : true;
}

void RankManager::RecordScore(const std::string& fileName, const std::string& nickName, const int& score)
{
	//READ
	std::ifstream fin(fileName);

	std::string sLine;
	std::vector<RankData> scores(5);

	int index = 0;
	while (!fin.eof() && index < 5)
	{
		fin >> sLine;

		if (sLine == "0:" || sLine == "")
		{
			scores[index++] = { 0,"" };
			continue;
		}

		int length = 0;
		for (const auto& it : sLine)
		{
			if (it != ':')
				length++;
			else
				break;
		}

		scores[index++] = { stoi(sLine.substr(0,length)),sLine.substr(length + 1) };
	}
	fin.close();

	//WRITE
	std::ofstream fout(fileName);
	int outCount = 0, outScore;
	bool isCompare = true, isNickName = false;

	for (index = 0; index < scores.size(); outCount++)
	{
		if (outCount == 5)break;

		if (isCompare && scores[index].score < score)
		{
			isCompare = false;
			isNickName = true;
			outScore = score;
		}
		else
			outScore = scores[index].score;

		if (isNickName)
		{
			isNickName = false;
			fout << outScore << ':' << nickName << ' ';
		}

		else
		{
			if (scores[index].nickName.empty())
			{
				while (outCount < 5)
				{
					fout << 0 << ':' << ' ';
					outCount++;
				}
				break;
			}

			else
				fout << outScore << ':' << scores[index++].nickName << ' ';
		}
	}
	fout.close();
}

std::vector<std::string> RankManager::GetRankData(const std::string& fileName)
{
	std::vector<std::string> output(5);
	std::vector<RankData> data(5);
	std::ifstream fin(fileName);
	
	std::string sLine;
	int index = 0;
	while (!fin.eof() && index < 5)
	{
		fin >> sLine;
		if (sLine == "0:" || sLine == "")
		{
			data[index++] = { 0,"" };
			continue;
		}
	
		int length = 0;
		for (const auto& it : sLine)
		{
			if (it != ':')
				length++;
			else
				break;
		}
	
		data[index++] = { stoi(sLine.substr(0,length)),sLine.substr(length + 1) };
	}
	
	fin.close();
	
	index = 0;
	for (const RankData& rank : data)
	{
		std::string write = std::to_string(rank.score) + '\t' + rank.nickName;
		output[index++] = write;
	}
	return output;
}