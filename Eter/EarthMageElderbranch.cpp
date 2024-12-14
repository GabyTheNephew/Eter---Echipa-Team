#include "EarthMageElderbranch.h"

const std::string EarthMageElderbranch::m_name = "Elderbranch";
const std::string EarthMageElderbranch::m_description = "Cover an opponent’s card with your card of a lower value.";

EarthMageElderbranch::EarthMageElderbranch()
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

bool EarthMageElderbranch::playMageElderbranch(Board& board, Color color, int16_t value, int16_t x, int16_t y)
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
