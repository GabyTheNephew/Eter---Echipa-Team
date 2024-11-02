#include "Board.h"

void Board::emptyRow(int row)
{
	for (auto& column : m_board[row])
	{
		column.clear();
	}
}

void Board::emptyColumn(int column)
{
	for (auto& row : m_board)
	{
		row[column].clear();
	}
}
