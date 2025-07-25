#pragma once

#include <string>

#include "Player.h"
class PowerHurricane
{
private:
	  const std::string m_name;
	  const std::string m_description;
public:
	PowerHurricane();


	std::string_view getName()const;
	std::string_view getDescription()const;

	bool checkHurricanePower(Board& board);
	bool playHurricanePower(Board& board, Player& player1, Player& player2, bool isRow, int16_t index, int16_t direction);

};

