#include "AirMageZephyraCrow.h"

const std::string AirMageZephyraCrow::m_name = "Zephyra Crow";
const std::string AirMageZephyraCrow::m_description = "Use your Mage Power card as an additional ETER card - place it on an empty space on the playing field with your marker.";

AirMageZephyraCrow::AirMageZephyraCrow()
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

bool AirMageZephyraCrow::playMageZephyraCrow(Board& board, Color color, int16_t x, int16_t y)
{
	if (board[{x,y}].empty())
	{
		board[{x,y}].push_back(EterCard(color)); ///needs to push EterCard insted of a simple card
		return true;
	}
	return false;
}
