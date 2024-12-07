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

	void playPower(Board& board, Player& player1, Player& player2,uint8_t value);
	bool checkPower(Board& board, uint8_t value);

};

