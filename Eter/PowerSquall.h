#pragma once

#include <iostream>
#include "Board.h"
#include "Player.h"
#include <string_view>
class PowerSquall
{
private:
	static const std::string m_name;
	static const std::string m_description;


public:
	PowerSquall();

	std::string getName() const;
	std::string getDescription() const;

	void playSquallPower(Board& board, Player& player1, Player& player2, int16_t x, int16_t y);
	bool checkSquallPower(Board& board, int16_t x, int16_t y);

};
