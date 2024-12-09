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

void PowerVifor::playPower(Board& board, Player& player1, Player& player2, int16_t x,int16_t y)//nrLvl - numarul de putere petru o carte
{
	
	if (ColorToString(board[{x, y}].back().getColor()) == "Red") {
		auto card = SimpleCard(board[{x, y}].back().getValue(), Color::Red);
		player1.makeCardValid(card);
		board.popCard({x, y});
	}
	else {
		if (ColorToString(board[{x, y}].back().getColor()) == "Blue") {
			auto card = SimpleCard(board[{x, y}].back().getValue(), Color::Blue);
			player1.makeCardValid(card);
			board.popCard({ x, y });
		}
	}

	
		
}

bool PowerVifor::checkPower(Board& board, int16_t x, int16_t y)
{
	if (board[{x, y}].back().getValue() != NULL)
	{
		return false;
	}

	return true;



}
