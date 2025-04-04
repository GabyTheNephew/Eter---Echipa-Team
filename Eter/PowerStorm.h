#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>

class PowerStorm
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerStorm();

	std::string getName() const;
	std::string getDescription() const;

	void playStormPower(Board& board, int16_t x, int16_t y);
	bool checkStormPower(Board& board, int16_t x, int16_t y);

};


