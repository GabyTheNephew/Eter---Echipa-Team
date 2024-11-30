#pragma once
#include<iostream>
#include"Player.h"
#include<string_view>


class PowerDestruction
{
private:
	std::string m_name;
	std::string m_desctiption;
public:
	PowerDestruction();
	PowerDestruction(std::string_view name, std::string_view description);

	std::string_view getName();
	std::string_view getDescription();

	bool checkPower(Player& player);
	void playPower(Player& player);
};

