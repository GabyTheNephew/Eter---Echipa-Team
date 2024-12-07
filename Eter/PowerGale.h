#pragma once
#include "Player.h"


#include <string>

class PowerGale
{
private:
	std::string m_name;
	std::string m_description;

public:
	PowerGale();

	std::string_view getName();
	std::string_view getDescription();

	bool checkPower(Player& player);
	void playPower(Board& board, Player& player1, Player& player2);
};


