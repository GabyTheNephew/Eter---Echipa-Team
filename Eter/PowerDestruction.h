#pragma once
#include<iostream>
#include"Player.h"
#include<string_view>


class PowerDestruction
{
private:
	static const std::string m_name;
	static const std::string m_desctiption;
public:
	PowerDestruction();

	std::string_view getName()const;
	std::string_view getDescription()const;

	bool checkDestructionPower(Player& player);
	void playDestructionPower(Player& player);
};

