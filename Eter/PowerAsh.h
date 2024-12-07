#pragma once

#include"Player.h"
#include<iostream>

class PowerAsh
{
private:
	std::string m_name;
	std::string m_description;

public:
	PowerAsh();
	PowerAsh(std::string_view name, std::string_view description);

	std::string_view getName();
	std::string_view getDescription();

	bool checkPower(Player& player);
	void playPower(Board& board,Player& player);
};

