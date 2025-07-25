#pragma once

#include"Player.h"
#include<iostream>

class PowerAsh
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerAsh();

	std::string_view getName()const;
	std::string_view getDescription()const;

	std::vector<int16_t> getEliminatedCardValues(Board& board, Player& player);
	bool checkAshPower(Board& board, Player& player);
	bool playAshPower(Board& board, Player& player, int16_t cardValue, int16_t x, int16_t y);
};

