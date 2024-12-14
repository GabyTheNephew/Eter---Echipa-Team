#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>
#include "Player.h"
#include "SimpleCard.h"

class PowerMist
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerMist();

	std::string getName()const;
	std::string getDescription()const;

	void playMistPower(Board& board, Player player, int16_t x, int16_t y);
	bool checkMistPower(Board& board, Player& player, int16_t x, int16_t y);

};
