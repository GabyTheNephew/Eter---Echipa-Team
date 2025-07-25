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
	if (x1 < 0 || x1 >= board.getRowSize() || y1 < 0 || y1 >= board.getColumnSize() || x2 < 0 || x2 >= board.getRowSize() || y2 < 0 || y2 >= board.getColumnSize()) 
	{
		return false;
	}

	if (x1 == x2 && y1 == y2) 
	{
		return false;
	}

	if (!board[{x1, y1}].empty() && !board[{x2, y2}].empty())
	{
		return true;
	}
	return false;
}

