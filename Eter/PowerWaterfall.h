#pragma once

#include"Player.h"
#include<iostream>
#include<string>
#include "Board.h"
#include <string_view>

class PowerWaterfall
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerWaterfall();

	std::string_view getName()const;
	std::string_view getDescription()const;

	
	bool checkWaterfallPower(Board& board);
	void PrintColumns(Board& board);
	void PrintRows(Board& board);
	void playWaterfallPower(Board& board, Player& player);
};


