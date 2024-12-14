#pragma once
#include <iostream>
#include "Player.h"
#include <string_view>

class PowerRock
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	PowerRock();

	std::string getName() const;
	std::string getDescription() const;

	void playRockPower(Board& board, Player& player, int16_t x, int16_t y);
	bool checkRockPower(Board& board, int16_t x, int16_t y);
};

