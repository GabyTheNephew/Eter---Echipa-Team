#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class EarthMageBumbleroot
{
private:

	std::string m_name;
	std::string m_description;

public:

	EarthMageBumbleroot(std::string_view name, std::string_view description);

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, int8_t x, int8_t y);
};

