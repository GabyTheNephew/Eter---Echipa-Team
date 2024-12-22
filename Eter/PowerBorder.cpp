#include "PowerBorder.h"

const std::string PowerBorder::m_name = "Board";
const std::string PowerBorder::m_description = "place this card in any empty space on the playing field to establish the border of the play area.Next, play your normal card.Any player may replace the Border card with any of their cards(it does not block the space).Condition - when played, ";

PowerBorder::PowerBorder()
{
}

std::string_view PowerBorder::getName() const
{
	return m_name;
}

std::string_view PowerBorder::getDescription() const
{
	return m_description;
}

