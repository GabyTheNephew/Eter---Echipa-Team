#include "PowerSquall.h"

const std::string PowerSquall::m_name = "Squall";
const std::string PowerSquall::m_description = "Return to the opponent’s hand any one of his visible cards on the playing field. ";

PowerSquall::PowerSquall()
{
}

std::string PowerSquall::getName() const
{
	return m_name;
}

std::string PowerSquall::getDescription() const
{
	return m_description;
}

void PowerSquall::playSquallPower(Board& board, Player& player1, Player& player2, int16_t x,int16_t y)//nrLvl - numarul de putere petru o carte
{
	
	if (ColorToString(board[{x, y}].back().getColor()) == "Red") {
		auto card = SimpleCard(board[{x, y}].back().getValue(), Color::Red);
		player1.makeCardValid(card);
		board.popCard({x, y});
	}
	else {
		if (ColorToString(board[{x, y}].back().getColor()) == "Blue") {
			auto card = SimpleCard(board[{x, y}].back().getValue(), Color::Blue);
			player2.makeCardValid(card);
			board.popCard({ x, y });
		}
	}

	
		
}

bool PowerSquall::checkSquallPower(Board& board, int16_t x, int16_t y)
{
	if (board[{x, y}].back().getValue() != NULL)
	{
		return false;
	}

	return true;



}
