#include "FireMageIgnara.h"


FireMageIgnara::FireMageIgnara(std::string_view name, std::string_view description) :
	m_name{"Ignara"},
	m_description{ "Remove from play an opponent’s card that covers one of your cards." }
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



bool FireMageIgnara::playMage(Board& board, std::string_view color, int8_t x, int8_t y)
{
	if (board.getBoard()[x][y].size() >= 2)
		if (board.getBoard()[x][y].back().getColor() != color && board.getBoard()[x][y][board.getBoard()[x][y].size()-2].getColor() == color)
		{
			board.getBoard()[x][y].pop_back();
			return true;
		}
	return false;
}
