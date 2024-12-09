#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>

class PowerTide
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	PowerTide();

	std::string getName() const;
	std::string getDescription() const;

	void playPower(Board& board, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
	bool checkPower(Board& board, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
};

