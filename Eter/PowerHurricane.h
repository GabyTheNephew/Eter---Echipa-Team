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

	bool checkHurricanePower(Board& board, Player& player);
	void playHurricanePower(Board& board, Player& player);
};

