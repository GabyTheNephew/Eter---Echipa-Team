#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>
#include "Player.h"
#include "SimpleCard.h"

class PowerBlizzard
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerBlizzard();

	std::string getName()const;
	std::string getDescription()const;

	void playBlizzardPower(Board& board, Player player1, Player player2, int16_t x, int16_t y, char position, std::vector<SimpleCard>& PastCards, std::optional<std::pair<bool, bool>>& canPlayIllusion);
	bool checkBlizzardPower(Board& board, Player& player, int16_t x, int16_t y);

};
