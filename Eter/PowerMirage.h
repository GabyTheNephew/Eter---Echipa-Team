#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>
#include "Player.h"
#include "SimpleCard.h"

class PowerMirage
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerMirage();

	std::string getName()const;
	std::string getDescription()const;

	void playMiragePower(Board& board, Player player, int16_t x, int16_t y);
	bool checkMiragePower(Board& board, Player& player, int16_t x, int16_t y);

};
