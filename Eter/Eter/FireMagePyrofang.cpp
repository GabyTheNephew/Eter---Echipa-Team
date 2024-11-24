#include "FireMagePyrofang.h"

FireMagePyrofang::FireMagePyrofang(std::string_view name, std::string_view description) :
	m_name{ "Pyrofang" },
	m_description{ "Remove from play a row or column of 3 or more cards. The removed row or column must contain at least one of your cards. The power impacts single cards and stacks." }
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

bool FireMagePyrofang::playMage(Board& board, std::string_view color, bool rowOrColumn, int8_t x)
{
	///row == false
	///column == true
	
	bool ok = false;

	if (rowOrColumn)
	{
		for (int i = 0; i < board.getSize(); i++)
		{
			if (ColorToString(board[{x, i}].value().getColor()) == color)
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
		for (int i = 0; i < board.getSize(); i++)
		{
			if (ColorToString(board[{i, x}].value().getColor()) == color)
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

