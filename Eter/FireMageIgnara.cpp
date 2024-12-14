#include "FireMageIgnara.h"

const std::string FireMageIgnara::m_name = "Ignara";
const std::string FireMageIgnara::m_description = "Remove from play an opponent’s card that covers one of your cards.";

FireMageIgnara::FireMageIgnara()
{

}

std::string FireMageIgnara::getName() const
{
	return m_name;
}

std::string FireMageIgnara::getDescription() const
{
	return m_description;
}



bool FireMageIgnara::playMageIgnara(Board& board, Color color, int16_t x, int16_t y)
{
	if (board[{x,y}].size() >= 2)
		if (board[{x,y}].back().getColor() != color && board[{x,y}][board[{x,y}].size()-2].getColor() == color)
		{
			board[{x,y}].pop_back();
			return true;
		}
	return false;
}
