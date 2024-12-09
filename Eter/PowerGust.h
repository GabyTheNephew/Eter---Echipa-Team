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

	bool checkPower(Board& board, int16_t x, int16_t y);
	void playPower(Board& board);
};


