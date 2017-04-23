#include "stdafx.h"
#include "csvReader.h"
#include <fstream>	//ifstream
#include <sstream>	//istringstream
#include <iomanip>	//getline

csvReader::csvReader(std::string fileName)
{
	InputFileStream = new std::ifstream(fileName);
	if (InputFileStream->fail()) {
		std::cout << "Arquivo nao encontrado!\n";
		system("pause");
		exit(0);
	}
}

std::vector<int>
csvReader::getNextLine(void)
{
	std::vector<int> thisTokenizedStream;

	std::string thisLine;
	if (std::getline(*InputFileStream, thisLine))
	{
		std::istringstream thisStream(thisLine);
		
		while (thisStream)
		{
			std::string thisToken;
			if (!getline(thisStream, thisToken, ','))
			{
				break;
			}
			else
			{
				thisTokenizedStream.push_back(std::stoi(thisToken));
			}
		}
	}

	return thisTokenizedStream;
}

std::vector<std::vector<int>>
csvReader::getAllLines(void)
{
	std::vector<std::vector<int>> thisTokenizedFile;

	while (InputFileStream)
	{
		std::string thisLine;
		if (std::getline(*InputFileStream, thisLine))
		{
			std::istringstream thisStream(thisLine);
			std::vector<int> thisTokenizedStream;
			while (thisStream)
			{
				std::string thisToken;
				if (!getline(thisStream, thisToken, ','))
				{
					break;
				}
				else
				{
					thisTokenizedStream.push_back(std::stoi(thisToken));
				}
			}
			thisTokenizedFile.push_back(thisTokenizedStream);
		}
		else
		{
			break;
		}
	}

	return thisTokenizedFile;
}

std::vector<std::vector<std::vector<int>>> csvReader::getAllSubLines(void)
{
	std::vector<std::vector<int>> thisTokenizedFile;
	std::vector<std::vector<std::vector<int>>> cells;

	while (InputFileStream)
	{
		std::string thisLine;
		if (std::getline(*InputFileStream, thisLine))
		{
			std::istringstream thisStream(thisLine);
			std::vector<int> thisTokenizedStream;
			while (thisStream)
			{
				std::string thisToken;
				if (!getline(thisStream, thisToken, ','))
				{
					break;
				}
				else
				{
					thisTokenizedStream.push_back(std::stoi(thisToken));
				}
			}
			thisTokenizedFile.push_back(thisTokenizedStream);
			cells.push_back(thisTokenizedFile);
			thisTokenizedFile.clear();
		}
		else
		{
			break;
		}
	}

	return cells;
}