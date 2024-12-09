#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>

class PowerSupport
{
private:

	std::string m_name;
	std::string m_description;

public:
	PowerSupport(std::string_view name, std::string_view description);

	std::string getName() const;
	std::string getDescription() const;

	void playPower(Board& board, int16_t x, int16_t y);
	bool checkPower(Board& board, int16_t x, int16_t y);

};
