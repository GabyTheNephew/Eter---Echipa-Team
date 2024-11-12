#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class AirMageVelora
{
private:

	std::string m_name;
	std::string m_description;

public:

	AirMageVelora(std::string_view name, std::string_view description);

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, std::string_view color, int8_t x, int8_t y, int8_t newX, int8_t newY);
};

