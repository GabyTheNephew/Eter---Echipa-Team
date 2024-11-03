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

Board::Board():
	 m_board{} //def constructor
{

}

Board::Board(uint8_t size)
{
	resizeBoard(size);
}

Board::Board(Board&& board) noexcept
	:m_board{ std::move(board.m_board) }
{

}

Board& Board::operator=(Board&& board) noexcept
{
	if (this == &board)
		return *this;

	m_board = std::move(board.m_board);
	return *this;
}

void Board::resizeBoard(int size)
{
	this->m_board.resize(size, std::vector<std::deque<int>>(size));
}


Board::Board(const Board& board)
	:m_board{board.m_board}
{}



Board& Board::operator=(const Board& board)
{
	if (this == &board)
	{
		return *this;
	}
	
	for (int i = 0; i < m_board.size(); i++)
	{
		for (int j = 0; j < m_board[i].size(); j++)
		{
			m_board[i][j] = board.m_board[i][j];
		}
	}
	return *this;
}
