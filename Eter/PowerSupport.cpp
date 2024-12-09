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

void PowerSupport::playSupportPower(Board& board, int16_t x, int16_t y)
{

	board[{x, y}].back().setValue(board[{x, y}].back().getValue()+1);
	Color color = board[{x, y}].back().getColor();
	if (ColorToString(color) == "Blue")
	{
		board[{x, y}].back().setColor(Color::increasedBlue);
	}
	if (ColorToString(color) == "Red")
	{
		board[{x, y}].back().setColor(Color::increasedRed);
	}
}

bool PowerSupport::checkSupportPower(Board& board, int16_t x, int16_t y)
{
	if (board[{x, y}].back().getValue() == 4)
	{
		return false;
	}
	return true;
}
