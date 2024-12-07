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

	void playPower(Board& board, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
	bool checkPower(Board& board, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
};

