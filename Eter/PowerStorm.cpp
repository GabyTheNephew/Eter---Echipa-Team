#include "PowerStorm.h"

PowerStorm::PowerStorm(std::string_view name, std::string_view description):
	m_name{ "STORM" },
	m_description{ "Remove from play Eany stack of minimum 2 cards. " }
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

void PowerStorm::playPower(Board& board, uint8_t x, uint8_t y)
{
	board[{x, y}].clear();
}

bool PowerStorm::checkPower(Board& board, uint8_t x, uint8_t y)
{
	if (board[{x, y}].size() >= 2)
	{
		return true;
	}
	return false;
}
