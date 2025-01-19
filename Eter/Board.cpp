#include "Board.h"

void Board::expandRow(RowExpandDirection direction)
{
	int16_t newSize = m_board.size() + 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(m_board[0].size()));

	if (direction == RowExpandDirection::Up)
	{
		for (int16_t i = newSize - 1; i > 0; --i)
		{
			for (int16_t j = 0; j < m_board[0].size(); j++)
			{
				newMatrix[i][j] = std::move(m_board[i - 1][j]);///////
			}
		}
	}
	else
	{
		for (int16_t i = 0; i < m_board.size(); ++i)
		{
			for (int16_t j = 0; j < m_board[i].size(); j++)
			{
				newMatrix[i][j] = std::move(m_board[i][j]);
			}
		}
	}

	m_board = std::move(newMatrix);
}

void Board::expandColumn(ColumnExpandDirection direction)
{
	int16_t newSize = m_board[0].size() + 1;
	matrix newMatrix(m_board.size(), std::vector<std::deque<SimpleCard>>(newSize));

	if (direction == ColumnExpandDirection::Left)
	{
		for (int16_t i = 0; i < m_board.size(); ++i)
		{
			for (int16_t j = newSize - 1; j > 0; --j)
			{
				newMatrix[i][j] = std::move(m_board[i][j - 1]);
			}
		}
	}
	else
	{
		for (int16_t i = 0; i < m_board.size(); ++i)
		{
			for (int16_t j = 0; j < m_board[0].size(); ++j)
			{
				newMatrix[i][j] = std::move(m_board[i][j]);
			}
		}
	}

	m_board = std::move(newMatrix);
}

std::vector<int16_t> Board::searchEmptyColumns()
{
	std::vector<int16_t> emptyCols;
	for (int16_t i = 0; i < getColumnSize(); i++)
	{
		bool isEmpty = true;
		for (int16_t j = 0; j < getRowSize(); j++)
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

std::vector<int16_t> Board::searchEmptyRows()
{
	std::vector<int16_t> emptyRows;
	for (int16_t i = 0; i < m_board.size(); i++)
	{
		bool isEmpty = true;
		for (int16_t j = 0; j < m_board[i].size(); j++)
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

bool Board::canBePlaced(int16_t x, int16_t y) const {
	int16_t rows = m_board.size();
	int16_t columns = m_board[0].size();

	/*if (x < 0 || x >= rows || y < 0 || y >= columns) {
		return false;
	}*/

	// Check neighbors
	bool check = false;
	std::vector<std::pair<int16_t, int16_t>> neighbors = {
		{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1},
		{x + 1, y + 1}, {x - 1, y + 1}, {x - 1, y - 1}, {x + 1, y - 1}
	};

	for (const auto& [nx, ny] : neighbors) {
		if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
			if (!m_board[nx][ny].empty()) {
				check = true;
				break;
			}
		}
	}
	

	if (m_size == 1)
	{
		return true;
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

Board::State Board::checkWin(bool canCountPoints)
{
	const int16_t kResults = 8;
	std::array<int16_t, kResults> results{};
	int16_t chessmanCount = 0;

	int16_t kRows = m_board.size();
	int16_t kColumns = m_board[0].size();

	for (int16_t i = 0; i < kRows; ++i)
	{
		for (int16_t j = 0; j < kColumns; ++j)
		{
			if (!m_board[i][j].empty())
			{
				int16_t value;
				Color color;
				color = m_board[i][j].back().getColor();

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
	if (chessmanCount == kRows * kColumns && canCountPoints)
	{
		int16_t redSum = sumPoints(Color::Red);
		int16_t blueSum = sumPoints(Color::Blue);

		if (redSum > blueSum)
		{
			return State::RedWin;
		}
		else
		{
			if (blueSum > redSum)
			{
				return State::BlueWin;
			}
		}

		return State::Draw;
	}

	return State::None;
}

int16_t Board::sumPoints(const Color& color)
{
	int16_t sum = 0;

	for (int16_t i = 0; i < m_board.size(); i++)
	{
		for (int16_t j = 0; j < m_board[i].size(); j++)
		{
			if (m_board[i][j].back().getColor() == color)
			{
				if (color == Color::IlusionBlue || color == Color::IlusionRed)
				{
					sum += 1;
				}
				else
				{
					sum += m_board[i][j].back().getValue();
				}
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



int16_t Board::getSize() const
{
	return m_size;
}

int16_t Board::getRowSize() const 
{
	return m_board.size();
}

int16_t Board::getColumnSize() const
{
	return m_board[0].size();
}

void Board::moveSpace(int16_t row, int16_t column, int16_t newRow, int16_t newColumn)
{
	m_board[newRow][newColumn] = std::move(m_board[row][column]);
	m_board[row][column].clear();
}

void Board::emptyRow(int16_t row)
{
	for (auto& column : m_board[row])
	{
		column.clear();
	}
}

void Board::emptyColumn(int16_t column)
{
	for (auto& row : m_board)
	{
		row[column].clear();
	}
}

void Board::removeRow(int16_t row)
{
	int16_t newSize = m_board.size() - 1;
	matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(m_board[0].size()));
	if (row == 0)
	{
		for (int16_t i = 0; i < newSize; i++)
		{
			for (int16_t j = 0; j < newMatrix[0].size(); j++)
			{
				newMatrix[i][j] = std::move(m_board[i + 1][j]);
			}
		}
	}
	else
	{
		if (row == m_board.size() - 1)
		{
			for (int16_t i = 0; i < newSize; i++)
			{
				for (int16_t j = 0; j < newMatrix[0].size(); j++)
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

void Board::removeColumn(int16_t column)
{
	int16_t newSize = m_board[0].size() - 1;
	matrix newMatrix(m_board.size(), std::vector<std::deque<SimpleCard>>(newSize));
	if (column == 0)
	{
		for (int16_t i = 0; i < newSize; i++)
		{
			for (int16_t j = 0; j < newSize; j++)
			{
				newMatrix[i][j] = std::move(m_board[i][j + 1]);
			}
		}
	}
	else
	{
		if (column == m_board[0].size() - 1)
		{
			for (int16_t i = 0; i < newSize; i++)
			{
				for (int16_t j = 0; j < newSize; j++)
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

Board::Board(int16_t size)
{
	this->resizeBoard(size);
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

void Board::resizeBoard(int16_t size)
{
	m_size = size;
	this->m_board.clear(); // Resetează complet matricea
	this->m_board.resize(size, std::vector<std::deque<SimpleCard>>(size));
}

void Board::print()const
{
	int16_t rows = m_board.size();
	int16_t cols = rows > 0 ? m_board[0].size() : 0;

	
	QDebug debug = qDebug().nospace();

	for (int16_t i = rows < 3 && cols < 3 ? -1 : 0; i <= rows; i++)
	{
		for (int16_t j = rows < 3 && cols < 3 ? -1 : 0; j <= cols; j++)
		{
			if(rows < 3 && cols < 3)
			{
				if (i < 0 || i >= rows || j < 0 || j >= cols)
				{
					// În afara matricei: verificăm dacă se poate plasa o carte
					if (canBePlaced(i, j))
					{
						debug << " * "; // Poziție eligibilă
					}
					else
					{
						debug << " "; // Poziție goală
					}
					continue;
				}
				else if (i >= 0 && i < rows && j>=0 && j < cols)
				{
					if(m_board[i][j].empty())
					{
						debug << " * ";
						continue;
					}
				}
			}

			if(j < cols && i < rows && i >= 0 && j >= 0)
			{
				if (!m_board[i][j].empty())
				{
					if (m_board[i][j].back().getColor() == Color::IlusionRed)
					{
						qDebug().noquote() << "iR" << " ";
					}
					else if (m_board[i][j].back().getColor() == Color::IlusionBlue)
					{
						qDebug().noquote() << "iB" << " ";
					}
					else
						debug << m_board[i][j].back().getValue() << " ";
				}
				else
				{
					debug << " * ";
				}
			}

		}

		debug << '\n';
	}
	qDebug() << '\n';
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

bool Board::checkRow(int16_t row)
{
	
	for (int16_t i = 0; i < m_board.size(); i++)
	{
		if (m_board[row][i].empty())
		{
			return false;
		}
	}
	return true;
}

bool Board::checkColumn(int16_t column)
{
	
	for (int16_t i = 0; i < m_board.size(); i++)
	{
		if (m_board[i][column].empty())
		{
			return false;
		}
	}
	return true;
}

void Board::pushCard(const SimpleCard& card, const Position& position)
{
	auto& [line, column] = position;

	if (canBePushed(card, position))
	{
		m_board[line][column].push_back(card);
	}
	else
	{
		std::cout << "Cannot push card because it has a smaller value\n";
	}
}

bool Board::canBePushed(const SimpleCard& card, const Position& position) const
{
	auto& [line, column] = position;

	if (m_board[line][column].empty())
	{
		return true;
	}

	if (card.getValue() > m_board[line][column].back().getValue())
	{
		return true;
	}

	return false;
}

void Board::popCard(const Position& position)
{
	auto& [line, column] = position;

	m_board[line][column].pop_back();
}

void Board::popCardAt(const Position& position, const SimpleCard& targetCard)
{
	auto& [line, column] = position;

	if (line > m_board.size() || column > m_board[0].size())
	{
		throw std::out_of_range("Position out of bounds");
	}

	std::deque<SimpleCard>& dequeWeRemoveFrom = m_board[line][column];

	for(auto it = dequeWeRemoveFrom.begin(); it != dequeWeRemoveFrom.end(); ++it)
	{
		if (it->getValue()==targetCard.getValue() && it->getColor()==targetCard.getColor())
		{
			dequeWeRemoveFrom.erase(it);
			break;
		}
	}
}

int Board::getNumberOfRowsWithCards() const
{
	int nr = 0;
	for (int i = 0;i < getRowSize();i++)
	{
		for (int j = 0;j < getColumnSize();j++)
		{
			if (!m_board[i][j].empty())
			{
				nr++;
				break;
			}
		}
	}
	return nr;
}

int Board::getNumberOfColumnsWithCards() const
{
	int nr = 0;
	for (int i = 0;i < getColumnSize();i++)
	{
		for (int j = 0;j < getRowSize();j++)
		{
			if (!m_board[j][i].empty())
			{
				nr++;
				break;
			}
		}
	}
	return nr;
}

bool Board::isFirstColumnEmpty() const
{
	for (int i = 0;i < m_board.size();i++)
	{
		if (!m_board[0][i].empty())
			return false;
	}
	return true;
}

bool Board::isLastColumnEmpty()const
{
	for (int i = 0;i < m_board.size();i++)
	{
		if (!m_board[i][getColumnSize() - 1].empty())
			return false;
	}
	return true;
}

bool Board::isFirstRowEmpty()const
{
	for (int i = 0;i < m_board[0].size();i++)
	{
		if (!m_board[0][i].empty())
			return false;
	}
	return true;
}

bool Board::isLastRowEmpty()const
{
	for (int i = 0;i < m_board[0].size();i++)
	{
		if (!m_board[getRowSize() - 1][i].empty())
			return false;
	}
	return true;
}

int Board::getIndexOfFirstRowOfBoard()
{
	int indexMin = 5;
	for (int i = 0;i < m_board.size();i++)
	{
		for (int j = 0;j < m_board[i].size();j++)
		{
			if (!m_board[i][j].empty())
			{
				if (i < indexMin)
					indexMin = i;
			}
		}
	}
	return indexMin;
}

int Board::getIndexOfLastRowOfBoard()
{
	int indexMax = 0;
	for (int i = 0;i < m_board.size();i++)
	{
		for (int j = 0;j < m_board[i].size();j++)
		{
			if (!m_board[i][j].empty())
			{
				if (i > indexMax)
					indexMax = i;
			}
		}
	}
	return indexMax;
}

int Board::getIndexOfFirstColumnOfBoard()
{
	int indexMin = 5;
	for (int i = 0;i < m_board.size();i++)
	{
		for (int j = 0;j < m_board[i].size();j++)
		{
			if (!m_board[i][j].empty())
			{
				if (j < indexMin)
					indexMin = j;
			}
		}
	}
	return indexMin;
}

int Board::getIndexOfLastColumnOfBoard()
{
	int indexMax = 0;
	for (int i = 0;i < m_board.size();i++)
	{
		for (int j = 0;j < m_board[i].size();j++)
		{
			if (!m_board[i][j].empty())
			{
				if (j > indexMax)
					indexMax = j;
			}
		}
	}
	return indexMax;
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
	
	for (int16_t i = 0; i < m_board.size(); i++)
	{
		for (int16_t j = 0; j < m_board[i].size(); j++)
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
	for (int16_t i = 0; i < board.m_board.size(); i++) {
		for (int16_t j = 0; j < board.m_board.size(); j++)
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
	for (int16_t i = 0; i < board.m_board.size(); i++) {
		for (int16_t j = 0; j < board.m_board.size(); j++)
		{
			int16_t val;
			in >> val;
			board.m_board[i][j].push_back(SimpleCard(val, Color::Red));
		}
	}
	return in;
}

std::deque<SimpleCard>& Board::operator[](const Board::Position& position)
{
	auto& [line, column] = position;

	int16_t kRows = m_board.size();
	int16_t kColumns = m_board[0].size();

	if (line < 0 || line > kRows || column < 0 || column > kColumns)
		throw std::out_of_range("Position out of bounds");

	return m_board[line][column];
}

const std::deque<SimpleCard>& Board::operator[](const Position& position) const
{
	auto& [line, column] = position;

	int16_t kRows = m_board.size();
	int16_t kColumns = m_board[0].size();

	if (line < 0 || line > kRows || column < 0 || column > kColumns)
		throw std::out_of_range("Position out of bounds");

	return m_board[line][column];
}