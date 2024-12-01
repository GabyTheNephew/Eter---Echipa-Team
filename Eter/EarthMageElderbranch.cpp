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

bool EarthMageElderbranch::playMage(Board& board, Color color, int8_t value, int8_t x, int8_t y)
{
	if ((board[{x,y}].back().getColor() != color) && (board[{x,y}].back().getValue() > value))
	{
		board[{x,y}].push_back(SimpleCard(value, color));
		if(color == Color::Red)
		{
			color = Color::usedRed;
		}
		else if (color == Color::Blue)
		{
			color = Color::usedBlue;
		}
		return true;
	}

	return false;
}
