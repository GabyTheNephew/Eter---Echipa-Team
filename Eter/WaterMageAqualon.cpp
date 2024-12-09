#include "WaterMageAqualon.h"

const std::string WaterMageAqualon::m_name = "Aqualon";
const std::string WaterMageAqualon::m_description = "Take any row or column of 3 or more cards at the edge of the playing field and move it to a different edge.This redefines the shape of the playing field. ";

WaterMageAqualon::WaterMageAqualon()
{
}

std::string WaterMageAqualon::getName() const
{
	return m_name;
}

std::string WaterMageAqualon::getDescription() const
{
	return m_description;
}

bool WaterMageAqualon::playMage(Board& board, bool rowOrColumn, int16_t x)
{
	///row == false
	///column == true

	if(board.getSize() >= 3 && (x==0 || x==board.getSize()))
	{
		if (rowOrColumn)
		{
			if (board.checkColumn(x))
			{
				for (int i = 0; i < board.getSize(); i++)
				{
					board.moveSpace(i, x, board.getSize()-i, x);
				}
				return true;
			}
		}
		else
		{
			if (board.checkRow(x))
			{
				for (int i = 0; i < board.getSize(); i++)
				{
					board.moveSpace(x, i, x, board.getSize()-i);
				}
				return true;
			}
		}
	}
	return false;
}

