#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class WaterMageAqualon
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	WaterMageAqualon();

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, Color color, bool rowOrColumn, int8_t x);
};

