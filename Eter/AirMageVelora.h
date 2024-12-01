#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class AirMageVelora
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	AirMageVelora();

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, Color color, int8_t x, int8_t y, int8_t newX, int8_t newY);
};
