#include "Board.h"

void Board::expandLeftUpCorner()
{
	uint8_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<std::optional<SimpleCard>>>(newSize));

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
	matrix newMatrix(newSize, std::vector<std::deque<std::optional<SimpleCard>>>(newSize));

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
	matrix newMatrix(newSize, std::vector<std::deque<std::optional<SimpleCard>>>(newSize));

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
	matrix newMatrix(newSize, std::vector<std::deque<std::optional<SimpleCard>>>(newSize));

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
	matrix newMatrix(m_board.size(), std::vector<std::deque<std::optional<SimpleCard>>>(newSize));

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
	matrix newMatrix(m_board.size(), std::vector<std::deque<std::optional<SimpleCard>>>(newSize));

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
	matrix newMatrix(newSize, std::vector<std::deque<std::optional<SimpleCard>>>(m_board[0].size()));

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
	matrix newMatrix(newSize, std::vector<std::deque<std::optional<SimpleCard>>>(m_board[0].size()));

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

bool Board::canBePlaced(int x, int y) const {
	uint8_t rows = m_board.size();
	uint8_t columns = m_board[0].size();

	if (x < 0 || x >= rows || y < 0 || y >= columns) {
		return false;
	}

	// Check neighbors
	std::vector<std::pair<int, int>> neighbors = {
		{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1},
		{x + 1, y + 1}, {x - 1, y + 1}, {x - 1, y - 1}, {x + 1, y - 1}
	};

	for (const auto& [nx, ny] : neighbors) {
		if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
			if (!m_board[nx][ny].empty()) {
				return true; 
			}
		}
	}

	
	return false;
}


//bool Board::lineWithColor(std::string_view Color) const
//{
//	bool check = true;
//	for (int i = 0; i < m_board.size(); i++)
//	{
//		check = true;
//
//		for (int j = 0; j < m_board[i].size(); j++)
//		{
//			if (m_board[i][j].front().getColor() != Color)
//			{
//				check = false;
//				break;
//			}
//		}
//	}
//	return check;
//}

//bool Board::columnWithColor(std::string_view Color) const
//{
//
//	bool check = true;
//	for (int i = 0; i < m_board.size(); i++)
//	{
//		check = true;
//		for (int j = 0; j < m_board[i].size(); j++)
//		{
//			if (m_board[j][i].front().getColor() != Color)
//			{
//				check = false;
//				break;
//			}
//		}
//	}
//	return check;
//}
//
//bool Board::diagonalWithColor(std::string_view Color) const
//{
//	bool check = true;
//	for(int i=0;i<m_board.size();i++)
//		if (m_board[i][i].front().getColor() != Color)
//		{
//			check = false;
//			break;
//		}
//
//	for (int i = 0; i < m_board.size(); i++)
//		if (m_board[i][m_board.size()-1-i].front().getColor() != Color)
//		{
//			check = false;
//			break;
//		}
//
//	return check;
//}

Board::State Board::checkWin()
{
	const uint8_t kResults = 8;
	std::array<int8_t, kResults> results{};
	int chessmanCount = 0;

	uint8_t kRows = m_board.size();
	uint8_t kColumns = m_board[0].size();

	for (uint32_t i = 0; i < kRows; ++i)
	{
		for (uint32_t j = 0; j < kColumns; ++j)
		{
			if (!m_board[i][j].empty())
			{
				int8_t value;
				Color color;
				if (m_board[i][j].back().has_value())
					color = m_board[i][j].back().value().getColor();

				switch (color)
				{
				case Color::Red:
					value = 1;
					break;
				case Color::Blue:
					value = -1;
					break;
				default:
					value = 0;
				}

				// rows
				results[i] += value;
				// columns
				results[3 + j] += value;
				// primary diagonal
				if (i == j)
					results[6] += value;
				// secondary diagonal
				if (i == kColumns - 1 - j)
					results[7] += value;

				++chessmanCount;
			}
		}
	}

	// check if we have winner
	for (auto result : results)
	{
		if (result == 3 || result == -3)
		{
			return State::Win;
		}
	}

	// check if it's a tie
	if (chessmanCount == kRows * kColumns)
	{
		int8_t redSum = sumPoints(Color::Red);
		int8_t blueSum = sumPoints(Color::Red);

		if (redSum != blueSum)
		{
			return State::Win;
		}

		return State::Draw;
	}

	return State::None;
}

int8_t Board::sumPoints(const Color& color)
{
	int8_t sum = 0;

	for (uint8_t i = 0; i < m_board.size(); i++)
	{
		for (uint8_t j = 0; j < m_board[i].size(); j++)
		{
			if (m_board[i][j].back().value().getColor() == color)
			{
				sum += m_board[i][j].back().value().getValue();
			}
		}
	}

	return sum;
}

//std::string_view Board::win()
//{
//	bool checkRed = lineWithColor("red") || columnWithColor("red")|| diagonalWithColor("red");
//	bool checkBlue = lineWithColor("blue") || columnWithColor("blue") || diagonalWithColor("blue");
//	if (checkRed)
//	{
//		return "Red";
//	}
//	if (checkBlue)
//	{
//		return "Blue";
//	}
//}



uint8_t Board::getSize() const
{
	return m_size;
}

void Board::moveSpace(uint8_t row, uint8_t column, uint8_t newRow, uint8_t newColumn)
{
	m_board[newRow][newColumn] = std::move(m_board[row][column]);
	m_board[row][column].clear();
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
	matrix newMatrix(newSize, std::vector<std::deque<std::optional<SimpleCard>>>(newSize + 1));

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
	matrix newMatrix(newSize + 1, std::vector<std::deque<std::optional<SimpleCard>>>(newSize));

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
	 m_board{},
	m_size{ 0 }//def constructor
{

}

Board::Board(uint8_t size)
{
	m_board.resize(size);
	m_size = size;
}

Board::Board(Board&& board) noexcept
	:m_board{ std::move(board.m_board) },
	m_size{ std::move(board.m_size) }
{

}

Board& Board::operator=(Board&& board) noexcept
{
	if (this == &board)
		return *this;

	m_board = std::move(board.m_board);
	m_size = std::move(board.m_size);

	return *this;
}

void Board::resizeBoard(uint8_t size)
{
	this->m_board.resize(size, std::vector<std::deque<std::optional<SimpleCard>>>(size));
}

void Board::print()const
{
	for (const auto& row : m_board)
	{
		for (const auto& column : row)
		{
			std::cout << column.back().value() << " ";
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

bool Board::checkRow(uint8_t row)
{
	bool check = true;
	for (int i = 0; i < m_board.size(); i++)
	{
		if (m_board[row][i].empty())
		{
			check = false;
		}
	}
	return check;
}

bool Board::checkColumn(uint8_t column)
{
	bool check = true;
	for (int i = 0; i < m_board.size(); i++)
	{
		if (m_board[i][column].empty())
		{
			check = false;
		}
	}
	return check;
}


Board::Board(const Board& board)
	:m_board{board.m_board},
	m_size{board.m_size}
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

	m_size = board.m_size;

	return *this;
}

//matrix& Board::getBoard()
//{
//	return m_board;
//}

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
				os << board.m_board[i][j].back().value() << " ";
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
			board.m_board[i][j].push_back(SimpleCard(val, Color::Red));
		}
	}
	return in;
}

std::optional<SimpleCard>& Board::operator[](const Board::Position& position)
{
	auto& [line, column] = position;

	uint8_t kRows = m_board.size();
	uint8_t kColumns = m_board[0].size();

	if (line < 0 || line > kRows || column < 0 || column > kColumns)
		throw std::out_of_range("Position out of bounds");

	return m_board[line][column].back();
}

const std::optional<SimpleCard>& Board::operator[](const Position& position) const
{
	auto& [line, column] = position;

	uint8_t kRows = m_board.size();
	uint8_t kColumns = m_board[0].size();

	if (line < 0 || line > kRows || column < 0 || column > kColumns)
		throw std::out_of_range("Position out of bounds");

	return m_board[line][column].back();
}