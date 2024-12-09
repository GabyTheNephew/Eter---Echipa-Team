#include "WaterMageChillThoughts.h"

const std::string WaterMageChillThoughts::m_name = "Chill Thoughts";
const std::string WaterMageChillThoughts::m_description = "On his turn, a Mage may move an opponent’s card (or a stack of cards with an opponent’s card) to a different, empty space.";

WaterMageChillThoughts::WaterMageChillThoughts()
{
}

std::string WaterMageChillThoughts::getName() const
{
	return m_name;
}

std::string WaterMageChillThoughts::getDescription() const
{
	return m_description;
}

bool WaterMageChillThoughts::playMage(Board& board, Color color, int16_t x, int16_t y, int16_t newX, int16_t newY)
{
	if (board[{x,y}].back().getColor() != color)
	{
		board.moveSpace(x, y, newX, newY);
		return true;
	}
	return false;
}


