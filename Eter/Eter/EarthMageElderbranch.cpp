#include "EarthMageElderbranch.h"

EarthMageElderbranch::EarthMageElderbranch(std::string_view name, std::string_view description):
	m_name{ "Elderbranch" },
	m_description{ "Cover an opponent’s card with your card of a lower value." }
{
}

std::string EarthMageElderbranch::getName() const
{
	return m_name;
}

std::string EarthMageElderbranch::getDescription() const
{
	return m_description;
}

bool EarthMageElderbranch::playMage(Board& board, std::string_view color, int8_t value, int8_t x, int8_t y)
{
	if ((board.getBoard()[x][y].back().getColor() != color) && (board.getBoard()[x][y].back().getValue() > value))
	{
		board.getBoard()[x][y].push_back(SimpleCard(value, color));
		if(color=="red")
		{
			color = "usedRed";
		}
		else if (color == "blue")
		{
			color = "usedBlue";
		}
		return true;
	}

	return false;
}
