#pragma once
#include <iostream>
#include "Board.h"
#include "Player.h"
#include <string_view>

class PowerFire
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	PowerFire();

	std::string getName() const;
	std::string getDescription() const;

	void playFirePower(Board& board, Player& player1, Player& player2, int16_t value);
	bool checkFirePower(Board& board, int16_t value);

};

