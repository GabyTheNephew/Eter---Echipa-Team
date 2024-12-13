#pragma once
#include"Player.h"
#include<iostream>

class PowerSpark
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerSpark();

	std::string_view getName()const;
	std::string_view getDescription()const;
	void printCoveredCards(Board& board, Player& player);
	bool checkSparkPower(Board& board, Player& player);
	void playSparkPower(Board& board, Player& player);
};


