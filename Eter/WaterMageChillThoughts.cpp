#include "WaterMageChillThoughts.h"

WaterMageChillThoughts::WaterMageChillThoughts(std::string_view name, std::string_view description):
	m_name{ "Chill Thoughts" },
	m_description{ "On his turn, a Mage may move an opponent’s card (or a stack of cards with an opponent’s card) to a different, empty space." }
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

bool WaterMageChillThoughts::playMage(Board& board, std::string_view color, int8_t x, int8_t y, int8_t newX, int8_t newY)
{
	if (board.getBoard()[x][y].back().getColor() != color)
	{
		board.moveSpace(x, y, newX, newY);
		return true;
	}
	return false;
}


