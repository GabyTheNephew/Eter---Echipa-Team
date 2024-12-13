#pragma once
#include"Player.h"
#include<iostream>

class PowerFlame
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerFlame();

	std::string_view getName()const;
	std::string_view getDescription()const;

	bool checkFlamePower(Board& board,Player& player);
	void playFlamePower(Board& board, Player& player);
};


