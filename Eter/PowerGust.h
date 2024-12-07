#pragma once
#include <string>

#include "Board.h"


class PowerGust
{
private:
	std::string m_name;
	std::string m_description;

public:
	PowerGust();

	std::string_view getName() const;
	std::string_view getDescription() const;

	bool checkPower(Board& board, uint8_t x, uint8_t y);
	void playPower(Board& board);
};


