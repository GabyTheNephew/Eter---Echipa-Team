#pragma once
#include <iostream>
#include "Board.h"
#include "Player.h"
#include <string_view>
class PowerWhirlpool
{
private:
	std::string m_name;
	std::string m_description;


public:
	PowerWhirlpool(std::string_view name, std::string_view description);

	std::string getName() const;
	std::string getDescription() const;

	void playPower(Board & board, int16_t xFirst, int16_t yFirst, int16_t xSecound, int16_t ySecound);
	bool checkPower(Board& board, int16_t xFirst, int16_t yFirst, int16_t xSecond, int16_t ySecond);
};

