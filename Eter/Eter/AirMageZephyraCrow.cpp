#include "AirMageZephyraCrow.h"

AirMageZephyraCrow::AirMageZephyraCrow(std::string_view name, std::string_view description):
	m_name{ "Zephyra Crow" },
	m_description{ "Use your Mage Power card as an additional ETER card - place it on an empty space on the playing field with your marker." }
{
}

std::string AirMageZephyraCrow::getName() const
{
	return m_name;
}

std::string AirMageZephyraCrow::getDescription() const
{
	return m_description;
}

bool AirMageZephyraCrow::playMage(Board& board, std::string_view color, int8_t x, int8_t y)
{
	if (board.getBoard()[x][y].empty())
	{
		board.getBoard()[x][y].push_back(EterCard(color));
		board.setBoard(board.getBoard());
	}
}
