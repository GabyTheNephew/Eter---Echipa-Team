#include "PowerRock.h"

const std::string PowerRock::m_name = "Rock";
const std::string PowerRock::m_description = "Cover any Illusion card with your card, without revealing the value of the Illusion. ";

PowerRock::PowerRock()
{
}

std::string PowerRock::getName() const
{
	return m_name;
}

std::string PowerRock::getDescription() const
{
	return m_description;
}

bool PowerRock::playRockPower(Board& board, Player& player, const SimpleCard& selectedCard, int16_t x, int16_t y)
{
	if (x < 0 || x >= board.getRowSize() || y < 0 || y >= board.getColumnSize()) 
	{
		return false;
	}

	if (!checkRockPower(board, x, y))
	{
		return false;
	}

	board[{x, y}].push_back(selectedCard);

	player.makeCardInvalid(selectedCard);
	player.getPastVector().push_back(selectedCard);

	return true;

}

bool PowerRock::checkRockPower(Board& board, int16_t x, int16_t y)
{
	if (x < 0 || x >= board.getRowSize() || y < 0 || y >= board.getColumnSize()) 
	{
		return false;
	}

	if (board[{x, y}].empty()) 
	{
		return false;
	}

	Color topCardColor = board[{x, y}].back().getColor();
	return (topCardColor == Color::IlusionRed || topCardColor == Color::IlusionBlue);

}
