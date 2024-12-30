#pragma once
#include <string>
#include <string_view>
#include "Player.h"

class PowerWave
{
private:
	 const std::string m_name;
	 const std::string m_description;
public:
	PowerWave();


	std::string_view getName()const;
	std::string_view getDescription()const;

	bool checkWavePower(Board& board, Player& player,int16_t x,int16_t y);
	void playWavePower(Board& board, Player& player, int16_t x, int16_t y);
	void printEmptyAdjacentStacks(Board& board, int16_t x, int16_t y);
};