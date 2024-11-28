#pragma once

#include <iostream>
#include "Board.h"
#include "Player.h"
#include <string_view>
class PowerVifor
{
private:
	std::string m_name;
	std::string m_description;


public:
	PowerVifor(std::string_view name, std::string_view description);

	std::string getName() const;
	std::string getDescription() const;

	void playPower(Board& board, Player& player1, Player& player2, uint8_t x, uint8_t y);
	bool checkPower(Board& board, uint8_t x, uint8_t y);

};
