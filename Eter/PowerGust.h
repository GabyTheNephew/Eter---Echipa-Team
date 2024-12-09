#pragma once
#include <string>

#include "Board.h"


class PowerGust
{
private:
	static const std::string m_name;
	static const std::string m_description;

public:
	PowerGust();

	std::string_view getName() const;
	std::string_view getDescription() const;

	bool checkGustPower(Board& board, int16_t x, int16_t y);
	void playGustPower(Board& board);
};


