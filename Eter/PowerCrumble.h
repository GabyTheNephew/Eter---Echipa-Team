#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>

class PowerCrumble
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerCrumble();

	std::string getName()const;
	std::string getDescription()const;

	void playCrumblePower(Board& board, int16_t x, int16_t y);
	bool checkCrumblePower(Board& board, int16_t x, int16_t y);

};
