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

void PowerTide::playTidePower(Board& board, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	std::swap(board[{x1, y1}], board[{x2, y2}]);
}

bool PowerTide::checkTidePower(Board& board, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	if (!board[{x1, y1}].empty() && !board[{x2, y2}].empty())
	{
		return true;
	}
	return false;
}

