#pragma once
#include "Board.h"
#include "Player.h"
#include <iostream>
#include <string_view>

class EarthMageElderbranch
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	EarthMageElderbranch();

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, Color color, int16_t value, int16_t x, int16_t y);
};

