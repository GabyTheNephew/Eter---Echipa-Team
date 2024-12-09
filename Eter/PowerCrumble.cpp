#include "PowerCrumble.h"

PowerCrumble::PowerCrumble(std::string_view name, std::string_view description) :
	m_name{ "Crumble" },
	m_description{ "Value of a card is decreased by 1 (cannot be used on a ‘1’ card).  Place your marker on the card. If the chosen card is ever covered or returned to your hand, the marker is removed. In case of a tie, the marker (if present on the playing f ield), is worth -1 point." }
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

void PowerCrumble::playPower(Board& board, int16_t x, int16_t y)
{

	board[{x, y}].back().setValue(board[{x, y}].back().getValue() - 1);
	Color color = board[{x, y}].back().getColor();
	if (ColorToString(color) == "Blue")
	{
		board[{x, y}].back().setColor(Color::decreasedBlue);
	}
	if (ColorToString(color) == "Red")
	{
		board[{x, y}].back().setColor(Color::decreasedRed);
	}
}

bool PowerCrumble::checkPower(Board& board, int16_t x, int16_t y)
{
	if (board[{x, y}].back().getValue() == 1)
	{
		return false;
	}
	return true;
}
