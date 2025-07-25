#include "PowerFlame.h"


const std::string PowerFlame::m_name = "Flame";
const std::string PowerFlame::m_description = "Reveal an opponent’s Illusion and next place your card on any space on the playing field.";


PowerFlame::PowerFlame()
{
}

std::string_view PowerFlame::getName() const
{
	return m_name;
}

std::string_view PowerFlame::getDescription() const
{
	return m_description;
}

bool PowerFlame::checkFlamePower(Board& board, Color playerColor)
{
	Color opponentIllusion = (playerColor == Color::Red) ? Color::IlusionBlue : Color::IlusionRed;

	for (int16_t i = 0; i < board.getRowSize(); i++)
	{
		for (int16_t j = 0; j < board.getColumnSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				if (board[{i, j}].back().getColor() == opponentIllusion)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool PowerFlame::playFlamePower(Board& board, Color playerColor, int16_t placeX, int16_t placeY, const SimpleCard& cardToPlace)
{
	Color opponentIllusion = (playerColor == Color::Red) ? Color::IlusionBlue : Color::IlusionRed;
	Color revealedColor = (opponentIllusion == Color::IlusionBlue) ? Color::Blue : Color::Red;

	bool illusionRevealed = false;
	for (int16_t i = 0; i < board.getRowSize() && !illusionRevealed; i++)
	{
		for (int16_t j = 0; j < board.getColumnSize() && !illusionRevealed; j++)
		{
			if (!board[{i, j}].empty() && board[{i, j}].back().getColor() == opponentIllusion)
			{
				board[{i, j}].back().setColor(revealedColor);
				illusionRevealed = true;
			}
		}
	}

	if (!illusionRevealed)
	{
		return false;
	}

	
	board.pushCard(cardToPlace, { placeX, placeY });
	return true;
}
