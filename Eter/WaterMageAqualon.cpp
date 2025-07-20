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

	if (rowOrColumn)
	{
		if ((x == 0 || x == board.getColumnSize() - 1) && board.checkColumn(x))
		{
			for (int i = 0; i < board.getRowSize(); ++i)
			{
				moved.push_back(board[{i, x}]);
			}

			if (x == 0)
			{
				board.removeColumn(0);
				board.expandColumn(Board::ColumnExpandDirection::Right);

				for (int i = 0; i < board.getRowSize(); i++)
				{
					board[{i, board.getColumnSize() - 1}] = moved[i];
				}
			}
			else
			{
				board.removeColumn(board.getColumnSize() - 1);
				board.expandColumn(Board::ColumnExpandDirection::Left);
				for (int i = 0; i < board.getRowSize(); i++)
				{
					board[{i, 0}] = moved[i];
				}
			}

			return true;


		}
	}
	else
	{
		if ((x == 0 || x == board.getRowSize() - 1) && board.checkRow(x))
		{
			for (int i = 0; i < board.getColumnSize(); ++i)
			{
				moved.push_back(board[{x, i}]);
			}
			if (x == 0)
			{
				board.removeRow(0);
				board.expandRow(Board::RowExpandDirection::Down);
				for (int i = 0; i < board.getColumnSize(); i++)
				{
					board[{board.getRowSize() - 1, i}] = moved[i];
				}
			}
			else
			{
				board.removeRow(board.getRowSize() - 1);
				board.expandRow(Board::RowExpandDirection::Up);
				for (int i = 0; i < board.getColumnSize(); i++)
				{
					board[{0, i}] = moved[i];
				}
			}
			return true;
		}
	}
	return false;
}

