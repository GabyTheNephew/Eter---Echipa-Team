#pragma once

#include"Player.h"
#include<iostream>

class PowerAsh
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerAsh();

	std::string_view getName()const;
	std::string_view getDescription()const;

	bool checkAshPower(Player& player);
	void playAshPower(Board& board,Player& player);
};

