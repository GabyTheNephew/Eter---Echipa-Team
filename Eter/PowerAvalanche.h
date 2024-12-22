#pragma once
#include"Player.h"
#include<iostream>
#include<string>

class PowerAvalanche
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerAvalanche();

	std::string_view getName()const;
	std::string_view getDescription()const;
	bool checkAvalanchePower(Board& board,bool rowColumn ,int16_t x1, int16_t y1, int16_t x2, int16_t y2);
	void playAvalanchePower(Board& board, Player& player, bool rowColumn, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
};

