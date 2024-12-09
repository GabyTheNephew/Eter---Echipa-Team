#pragma once
#include <iostream>
#include "Board.h"
#include "Player.h"
#include <string_view>
class PowerWhirlpool
{
private:
	static const std::string m_name;
	static const std::string m_description;


public:
	PowerWhirlpool();

	std::string getName() const;
	std::string getDescription() const;

	void playWhirlpoolPower(Board & board, int16_t xFirst, int16_t yFirst, int16_t xSecound, int16_t ySecound);
	bool checkWhirlpoolPower(Board& board, int16_t xFirst, int16_t yFirst, int16_t xSecond, int16_t ySecond);
};

