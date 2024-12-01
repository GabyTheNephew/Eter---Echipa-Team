#pragma once
#include "Board.h"
#include "EterCard.h"
#include <iostream>
#include <string_view>


class AirMageZephyraCrow
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	AirMageZephyraCrow();

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, Color color, int8_t x, int8_t y);
};

