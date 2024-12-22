#pragma once
#include"Player.h"
#include<iostream>
#include<string>

class PowerBorder
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerBorder();

	std::string_view getName()const;
	std::string_view getDescription()const;
	bool checkBorderPower(Board& board);
	void playBorderPower(Board& board, Player& player);
};

