#include "Board.h"
#include "Board.h"
#include "Board.h"

void Board::expandLeftUpCorner()
{
	uint8_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(newSize));

	for (int8_t i = m_board.size() - 1; i >= 0 ; --i)
	{
		for (int8_t j = m_board.size() - 1; j >= 0; --j)
		{
			newMatrix[i + 1][j + 1] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandRightUpCorner()
{
	uint8_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(newSize));

	for (int8_t i = m_board.size() - 1; i >= 0; --i)
	{
		for (int8_t j = m_board.size() - 1; j >= 0; --j)
		{
			newMatrix[i + 1][j] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandLeftBottomCorner()
{
	uint8_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(newSize));

	for (int8_t i = m_board.size() - 1; i >= 0; --i)
	{
		for (int8_t j = m_board.size() - 1; j >= 0; --j)
		{
			newMatrix[i][j+1] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandRightBottomCorner()
{
	uint8_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(newSize));

	for (int8_t i = m_board.size() - 1; i >= 0; --i)
	{
		for (int8_t j = m_board.size() - 1; j >= 0; --j)
		{
			newMatrix[i][j] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandRight()
{
	uint8_t newSize = m_board[0].size() + 1;
	matrix newMatrix(m_board.size(), std::vector<std::deque<SimpleCard>>(newSize));

	for (int8_t i = 0; i < m_board.size() ; ++i)
	{
		for (int8_t j = 0; j < m_board[0].size(); ++j)
		{
			newMatrix[i][j] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandLeft()
{
	uint8_t newSize = m_board[0].size() + 1;
	matrix newMatrix(m_board.size(), std::vector<std::deque<SimpleCard>>(newSize));

	for (int8_t i = 0; i < m_board.size(); ++i)
	{
		for (int8_t j = m_board[0].size() - 1 ;j >= 1; --j)
		{
			newMatrix[i][j] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandDown()
{
	uint8_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(m_board[0].size()));

	for (int8_t i = 0; i < m_board.size(); ++i)
	{
		for (int8_t j = 0; j < m_board[i].size(); j++)
		{
			newMatrix[i][j] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandUp()
{
	uint8_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(m_board[0].size()));

	for (int8_t i = m_board.size()-1; i >= 1; --i)
	{
		for (int8_t j = 0; j < m_board[i].size(); j++)
		{
			newMatrix[i][j] = std::move(m_board[i][j]);
		}
	}

	m_board = std::move(newMatrix);
}

std::vector<uint8_t> Board::searchEmptyColumns()
{
	std::vector<uint8_t> emptyCols;
	for (int i = 0; i < m_board.size(); i++)
	{
		bool isEmpty = true;
		for (int j = 0; j < m_board[i].size(); j++)
		{
			if (m_board[j][i].size() != 0)
				isEmpty = false;
		}

		if (isEmpty)
		{
			emptyCols.push_back(i);
		}
	}

	return emptyCols;
}

std::vector<uint8_t> Board::searchEmptyRows()
{
	std::vector<uint8_t> emptyRows;
	for (int i = 0; i < m_board.size(); i++)
	{
		bool isEmpty = true;
		for (int j = 0; j < m_board[i].size(); j++)
		{
			if (m_board[i][j].size() != 0)
				isEmpty = false;
		}

		if (isEmpty)
		{
			emptyRows.push_back(i);
		}
	}

	return emptyRows;
}

void Board::emptyRow(uint8_t row)
{
	for (auto& column : m_board[row])
	{
		column.clear();
	}
}

void Board::emptyColumn(uint8_t column)
{
	for (auto& row : m_board)
	{
		row[column].clear();
	}
}

void Board::removeRow(uint8_t row)
{
	uint8_t newSize = m_board.size() - 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(newSize + 1));

	if (row == 0)
	{
		for (int8_t i = 0; i < newSize; i++)
		{
			for (int8_t j = 0; j < newSize; j++)
			{
				newMatrix[i][j] = std::move(m_board[i + 1][j]);
			}
		}
	}
	else
	{
		if (row == m_board.size() - 1)
		{
			for (int8_t i = 0; i < newSize; i++)
			{
				for (int8_t j = 0; j < newSize; j++)
				{
					newMatrix[i][j] = std::move(m_board[i][j]);
				}
			}
		}
		else
		{
			std::cout << "Nu se poate sterge randul din mijloc!\n";
		}
	}

	m_board = std::move(newMatrix);
}

void Board::removeColumn(uint8_t column)
{
	uint8_t newSize = m_board[0].size() - 1;
	matrix newMatrix(newSize + 1, std::vector<std::deque<SimpleCard>>(newSize));

	if (column == 0)
	{
		for (int8_t i = 0; i < newSize; i++)
		{
			for (int8_t j = 0; j < newSize; j++)
			{
				newMatrix[i][j] = std::move(m_board[i][j + 1]);
			}
		}
	}
	else
	{
		if (column == newSize - 1)
		{
			for (int8_t i = 0; i < newSize; i++)
			{
				for (int8_t j = 0; j < newSize; j++)
				{
					newMatrix[i][j] = std::move(m_board[i][j]);
				}
			}
		}
		else
		{
			std::cout << "Nu se poate sterge coloana din mijloc!\n";
		}
	}

	m_board = std::move(newMatrix);
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

void Board::resizeBoard(uint8_t size)
{
	this->m_board.resize(size, std::vector<std::deque<SimpleCard>>(size));
}

void Board::print()const
{
	for (const auto& row : m_board)
	{
		for (const auto& column : row)
		{
			std::cout << column.back() << " ";
		}
	}
	std::cout << std::endl;
}

void Board::clear()
{
	for (auto& row : m_board)
	{
		for (auto& column : row)
		{
			column.clear();
		}
	}
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

matrix& Board::getBoard()
{
	return m_board;
}

void Board::setBoard(const matrix& board)
{
	m_board = board;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
	for (int i = 0; i < board.m_board.size(); i++) {
		for (int j = 0; j < board.m_board.size(); j++)
		{
			if (board.m_board[i][j].empty() == true) {
				os << 0 << " ";
			}
			else {
				os << board.m_board[i][j].back() << " ";
			}
		}
		os << '\n';
	}
	
	return os;
}

std::istream& operator>>(std::istream& in, Board& board)
{
	for (int i = 0; i < board.m_board.size(); i++) {
		for (int j = 0; j < board.m_board.size(); j++)
		{
			int8_t val;
			in >> val;
			board.m_board[i][j].push_back(SimpleCard(val, "r"));
		}
	}
	return in;
}
