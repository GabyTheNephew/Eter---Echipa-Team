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

bool AirMageVelora::playMageVelora(Board& board, Color color, int16_t x, int16_t y, int16_t newX, int16_t newY)
{
	if(!board[{x,y}].empty())
	{
		if ((board[{x, y}].back().getColor() == color) && (board[{x, y}].back().getColor() != Color::Hole))
		{
			board.moveSpace(x, y, newX, newY);
			return true;
		}
	}
	return false;
}
