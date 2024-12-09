#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class FireMageIgnara
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	FireMageIgnara();

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, Color color, int16_t x, int16_t y);
};

