#include "AirMageVelora.h"

const std::string AirMageVelora::m_name = "Velora";
const std::string AirMageVelora::m_description = "On his turn, the player may move his card (or a stack of cards with his card on top) to a different, empty space.";

AirMageVelora::AirMageVelora()
{
}

std::string AirMageVelora::getName() const
{
	return m_name;
}

std::string AirMageVelora::getDescription() const
{
	return m_description;
}

bool AirMageVelora::playMage(Board& board, Color color, int8_t x, int8_t y, int8_t newX, int8_t newY)
{
	if (board[{x,y}].back().getColor() == color)
	{
		board.moveSpace(x, y, newX, newY);
		return true;
	}
	return false;
}
