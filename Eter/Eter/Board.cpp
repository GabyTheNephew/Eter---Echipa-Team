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

Board::Board(const Board& board)
{
	for (int i = 0; i < m_board.size(); i++)
	{
		for (int j = 0; j < m_board[i].size(); j++)
		{
			m_board[i][j] = board.m_board[i][j];
		}
	}
}
