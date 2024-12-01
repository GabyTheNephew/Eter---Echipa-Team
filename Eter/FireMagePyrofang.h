#pragma once
#include "Board.h"
#include <iostream>
#include <string_view>

class FireMagePyrofang
{
private:

	std::string m_name;
	std::string m_description;

public:

	FireMagePyrofang(std::string_view name, std::string_view description);

	std::string getName() const;
	std::string getDescription() const;

	bool playMage(Board& board, Color color, bool rowOrColumn ,int8_t x);
};

