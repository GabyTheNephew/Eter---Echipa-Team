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

void PowerSquall::playSquallPower(Board& board, Player& player1, Player& player2, const std::string& currentPlayerColor, int16_t x, int16_t y)
{
	SimpleCard cardToReturn = board[{x, y}].back();

	Color originalColor = (cardToReturn.getColor() == Color::Red || cardToReturn.getColor() == Color::usedRed)? Color::Red : Color::Blue;
	

	SimpleCard returnCard(cardToReturn.getValue(), originalColor);


	if (currentPlayerColor == "Red") 
	{
		
		player2.makeCardValid(returnCard);
	}
	else 
	{
		player1.makeCardValid(returnCard);
	}

	board.popCard({ x, y });
}

bool PowerSquall::checkSquallPower(Board& board, const std::string& currentPlayerColor, int16_t x, int16_t y)
{
	if(x<0 || x >= board.getRowSize() || y < 0 || y >= board.getColumnSize()) 
	{
	
		return false;
	}
	if(board[{x, y}].empty()) 
	{
		return false;
	}

	Color cardColor = board[{x, y}].back().getColor();

	if ((currentPlayerColor == "Red" && (cardColor == Color::Blue || cardColor == Color::IlusionBlue)) || (currentPlayerColor == "Blue" && (cardColor == Color::Red || cardColor == Color::increasedRed)))
	{
		return true;
	}

	return false;

}
