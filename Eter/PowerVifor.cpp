#include "PowerVifor.h"

PowerVifor::PowerVifor(std::string_view name, std::string_view description) :
	m_name{ "SQUALL" },
	m_description{ "Return to the opponent’s hand any one of his visible cards on the playing field. " }
{
}

std::string PowerVifor::getName() const
{
	return m_name;
}

std::string PowerVifor::getDescription() const
{
	return m_description;
}

void PowerVifor::playPower(Board& board, Player& player1, Player& player2, uint8_t x,uint8_t y)//nrLvl - numarul de putere petru o carte
{
	
	if (ColorToString(board[{x, y}].getColor()) == "Red") {
		auto card = SimpleCard(board[{x, y}].getValue(), Color::Red);
		player1.makeCardValid(card);
		board.popCard({x, y});
	}
	else {
		if (ColorToString(board[{x, y}].getColor()) == "Blue") {
			auto card = SimpleCard(board[{x, y}].getValue(), Color::Blue);
			player1.makeCardValid(card);
			board.popCard({ x, y });
		}
	}

	
		
}

bool PowerVifor::checkPower(Board& board, uint8_t x, uint8_t y)
{
	if (board[{x, y}].getValue() != NULL)
	{
		return false;
	}

	return true;



}
