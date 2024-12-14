#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class FireMagePyrofang
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:

	FireMagePyrofang();

	std::string getName() const;
	std::string getDescription() const;

	bool playMagePyrofang(Board& board, Color color, bool rowOrColumn ,int16_t x);
};


