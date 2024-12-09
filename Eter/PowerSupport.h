#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>

class PowerSupport
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerSupport();

	std::string getName() const;
	std::string getDescription() const;

	void playSupportPower(Board& board, int16_t x, int16_t y);
	bool checkSupportPower(Board& board, int16_t x, int16_t y);

};
