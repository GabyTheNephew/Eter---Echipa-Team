#include "PowerCrumble.h"



const std::string PowerCrumble::m_name = "Crumble";
const std::string PowerCrumble::m_description = "Value of a card is decreased by 1 (cannot be used on a ‘1’ card).  Place your marker on the card. If the chosen card is ever covered or returned to your hand, the marker is removed. In case of a tie, the marker (if present on the playing f ield), is worth -1 point.";

PowerCrumble::PowerCrumble()
{
}

std::string PowerCrumble::getName() const
{
	return m_name;
}

std::string PowerCrumble::getDescription() const
{
	return m_description;
}

bool PowerCrumble::playCrumblePower(Board& board, Color playerColor, int16_t x, int16_t y)
{

	if (!checkCrumblePower(board, playerColor, x, y))
	{
		return false;
	}

	SimpleCard& targetCard = board[{x, y}].back();
	targetCard.setValue(targetCard.getValue() - 1);

	
	if (playerColor == Color::Red) 
	{
		targetCard.setColor(Color::decreasedBlue);
	}
	else if (playerColor == Color::Blue) 
	{
		targetCard.setColor(Color::decreasedRed);
	}

	return true;
}

bool PowerCrumble::checkCrumblePower(Board& board, Color playerColor, int16_t x, int16_t y)
{
	if (x < 0 || x >= board.getRowSize() || y < 0 || y >= board.getColumnSize())
	{
		return false;
	}

	if (board[{x, y}].empty())
	{
		return false;
	}

	SimpleCard& targetCard = board[{x, y}].back();
	if (targetCard.getValue() == 1)
	{
		return false;
	}

	Color cardColor = targetCard.getColor();
	if (playerColor == Color::Red)
	{
		if (cardColor != Color::Blue && cardColor != Color::decreasedBlue)
		{
			return false;
		}
	}
	else 
		if (playerColor == Color::Blue)
	{
	
		if (cardColor != Color::Red && cardColor != Color::decreasedRed)
		{
			return false;
		}
	}
	if (cardColor == Color::decreasedRed || cardColor == Color::decreasedBlue)
	{
		return false;
	}

	return true;
}
