#pragma once
#include "Player.h"


#include <string>

class PowerGale
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerGale();

	std::string_view getName()const;
	std::string_view getDescription()const;

	bool checkGalePower(Player& player);
	void playGalePower(Board& board, Player& player1, Player& player2);
};


