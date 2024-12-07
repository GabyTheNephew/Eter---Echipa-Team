#include "PowerTide.h"

const std::string PowerTide::m_name = "Tide";
const std::string PowerTide::m_description = "Change positions of 2 different cards or stacks of cards.";

PowerTide::PowerTide()
{
}

std::string PowerTide::getName() const
{
	return m_name;
}

std::string PowerTide::getDescription() const
{
	return m_description;
}

void PowerTide::playPower(Board& board, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	std::swap(board[{x1, y1}], board[{x2, y2}]);
}

bool PowerTide::checkPower(Board& board, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	if (!board[{x1, y1}].empty() && !board[{x2, y2}].empty())
	{
		return true;
	}
	return false;
}

