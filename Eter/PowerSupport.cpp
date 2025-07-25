#include "PowerSupport.h"

const std::string PowerSupport::m_name = "SUPPORT";
const std::string PowerSupport::m_description = "Value of a chosen card is increased by +1 (cannot be used on a ‘4’ card). Place your marker on the card. If the chosen card is ever covered or returned to your hand, the marker is removed. In case of a tie, the marker (if present on the playing field), is worth +1 point.";


PowerSupport::PowerSupport()
{
}

std::string PowerSupport::getName() const
{
	return m_name;
}

std::string PowerSupport::getDescription() const
{
	return m_description;
}

bool PowerSupport::playSupportPower(Board& board, Color playerColor, int16_t x, int16_t y)
{

	if (!checkSupportPower(board, playerColor, x, y))
	{
		return false;
	}

	SimpleCard& targetCard = board[{x, y}].back();
	targetCard.setValue(targetCard.getValue() + 1);

	if (playerColor == Color::Red)
	{
		targetCard.setColor(Color::increasedRed);
	}
	else if (playerColor == Color::Blue)
	{
		targetCard.setColor(Color::increasedBlue);
	}

	return true;
}

bool PowerSupport::checkSupportPower(Board& board, Color playerColor, int16_t x, int16_t y)
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


	if (targetCard.getValue() == 4)
	{
		return false;
	}

	
	Color cardColor = targetCard.getColor();
	if (playerColor == Color::Red)
	{
		if (cardColor != Color::Red && cardColor != Color::increasedRed)
		{
			return false;
		}
	}
	else if (playerColor == Color::Blue)
	{
		if (cardColor != Color::Blue && cardColor != Color::increasedBlue)
		{
			return false;
		}
	}

	
	if (cardColor == Color::increasedRed || cardColor == Color::increasedBlue)
	{
		return false;
	}

	return true;
}
