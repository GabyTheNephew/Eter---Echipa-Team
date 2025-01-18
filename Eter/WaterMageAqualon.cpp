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

bool WaterMageAqualon::playMageAqualon(Board& board, bool rowOrColumn, int16_t x)
{
	///row == false
	///column == true

	std::vector<std::deque<SimpleCard>> moved;

	if(board.getSize() >= 3 && (x==0 || x==board.getSize()-1))
	{
		if (rowOrColumn)
		{
			if (board.checkColumn(x))
			{
				for (int i = 0; i < board.getSize(); i++)
				{
					moved.push_back(board[{i, x}]);
				}

				if (x == 0)
				{
					board.removeColumn(0);
					board.expandColumn(Board::ColumnExpandDirection::Right);
				}
				else if (x == board.getSize() - 1)
				{
					board.removeColumn(board.getSize() - 1);
					board.expandColumn(Board::ColumnExpandDirection::Left);
				}

				for (int i = 0; i < board.getSize(); i++)
				{
					board[{i, board.getSize() - 1 - x}] = moved[i];
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
					moved.push_back(board[{x, i}]);
				}

				if (x == 0)
				{
					board.removeRow(0);
					board.expandRow(Board::RowExpandDirection::Down);
				}
				else if (x == board.getSize() - 1)
				{
					board.removeRow(board.getSize() - 1);
					board.expandRow(Board::RowExpandDirection::Up);
				}

				for (int i = 0; i < board.getSize(); i++)
				{
					board[{board.getSize() - 1 - x, i}] = moved[i];
				}

				return true;
			}
		}
	}
	return false;
}

