#include "FireMagePyrofang.h"

const std::string FireMagePyrofang::m_name = "Pyrofang";
const std::string FireMagePyrofang::m_description = "Remove from play a row or column of 3 or more cards. The removed row or column must contain at least one of your cards. The power impacts single cards and stacks.";

FireMagePyrofang::FireMagePyrofang()
{
}

std::string FireMagePyrofang::getName() const
{
	return m_name;
}

std::string FireMagePyrofang::getDescription() const
{
	return m_description;
}

bool FireMagePyrofang::playMagePyrofang(Board& board, Color color, bool rowOrColumn, int16_t x)
{
	///row == false
	///column == true
	
	bool ok = false;

	if (rowOrColumn)
	{
		for (int16_t i = 0; i < board.getSize(); i++)
		{
			if (board[{x, i}].back().getColor() == Color::Hole)
			{
				break;
			}
			else if (board[{x, i}].back().getColor() == color)
			{
				ok = true;
			}
		}
		if (ok)
		{
			board.removeColumn(x);
			return true;
		}
	}
	else
	{
		for (int16_t i = 0; i < board.getSize(); i++)
		{
			if (board[{x, i}].back().getColor() == Color::Hole)
			{
				break;
			}
			else if (board[{i, x}].back().getColor() == color)
			{
				ok = true;
			}
		}
		if (ok)
		{
			board.removeRow(x);
			return true;
		}
	}
	return false;
}

