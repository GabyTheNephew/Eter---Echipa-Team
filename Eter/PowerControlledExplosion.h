#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>
#include "Player.h"
#include "SimpleCard.h"

class PowerControlledExplosion
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerControlledExplosion();

	std::string getName()const;
	std::string getDescription()const;

	void playControlledExplosioPower(Board& board, Player player, int16_t x, int16_t y);
	bool checkControlledExplosioPower(Board& board, Player& player, int16_t x, int16_t y);

};
