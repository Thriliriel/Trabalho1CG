#pragma once

#include <string>
#include <vector>

class csvReader
{
private:
	std::ifstream* InputFileStream;
public:
	csvReader(std::string fileName);
	std::vector<int> getNextLine(void);
	std::vector<std::vector<int>> getAllLines(void);
};