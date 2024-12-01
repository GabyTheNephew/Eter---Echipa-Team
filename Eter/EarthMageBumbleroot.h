#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class EarthMageBumbleroot
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	EarthMageBumbleroot();

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, int8_t x, int8_t y);
};

