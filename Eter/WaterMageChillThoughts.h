#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class WaterMageChillThoughts
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	WaterMageChillThoughts();

	std::string getName() const;
	std::string getDescription() const;

	bool playMageChillThoughts(Board& board, Color color, int16_t x, int16_t y, int16_t newX, int16_t newY);
};

