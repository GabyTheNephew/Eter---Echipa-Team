#include "PowerStorm.h"


const std::string PowerStorm::m_name = "STORM";
const std::string PowerStorm::m_description = "Remove from play Eany stack of minimum 2 cards.";

PowerStorm::PowerStorm()
{
}

std::string PowerStorm::getName() const
{
	return m_name;
}

std::string PowerStorm::getDescription() const
{
	return m_description;
}

void PowerStorm::playStormPower(Board& board, int16_t x, int16_t y)
{
	board[{x, y}].clear();
}

bool PowerStorm::checkStormPower(Board& board, int16_t x, int16_t y)
{
	if (board[{x, y}].size() >= 2)
	{
		return true;
	}
	return false;
}
