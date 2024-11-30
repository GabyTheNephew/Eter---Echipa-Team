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

	void playPower(Board & board, uint8_t xFirst, uint8_t yFirst, uint8_t xSecound, uint8_t ySecound);
	bool checkPower(Board& board, uint8_t xFirst, uint8_t yFirst, uint8_t xSecond, uint8_t ySecond);
};

