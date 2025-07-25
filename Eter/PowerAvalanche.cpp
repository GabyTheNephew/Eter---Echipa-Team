#include "PowerAvalanche.h"

const std::string PowerAvalanche::m_name = "Avalanche";
const std::string PowerAvalanche::m_description = "Move two neighboring cards / stacs by one space horizontally or vertically.The space that the first card moves onto must be empty.";

PowerAvalanche::PowerAvalanche()
{
}

std::string_view PowerAvalanche::getName() const
{
	return m_name;
}

std::string_view PowerAvalanche::getDescription() const
{
	return m_description;
}

bool PowerAvalanche::checkAvalanchePower(Board& board, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t direction)
{
	if (x1 < 0 || x1 >= board.getRowSize() || y1 < 0 || y1 >= board.getColumnSize() ||x2 < 0 || x2 >= board.getRowSize() || y2 < 0 || y2 >= board.getColumnSize())
	{
		return false;
	}


	if (board[{x1, y1}].empty() || board[{x2, y2}].empty())
	{
		return false;
	}

	
	int16_t rowDiff = std::abs(x1 - x2);
	int16_t colDiff = std::abs(y1 - y2);
	if (!((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1)))
	{
		return false;
	}


	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };

	if (x1 == x2)
	{
		if (direction != 2 && direction != 3) 
		{
			return false;
		}
	}
	else if (y1 == y2) 
	{
		if (direction != 0 && direction != 1) 
		{
			return false;
		}
	}

	int16_t newX1 = x1 + dx[direction];
	int16_t newY1 = y1 + dy[direction];

	if (newX1 < 0 || newX1 >= board.getRowSize() || newY1 < 0 || newY1 >= board.getColumnSize())
	{
		return false;
	}

	if (!board[{newX1, newY1}].empty())
	{
		return false;
	}

	return true;
}

bool PowerAvalanche::playAvalanchePower(Board& board, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t direction)
{
	if (!checkAvalanchePower(board, x1, y1, x2, y2, direction))
	{
		return false;
	}

	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };

	
	int16_t newX1 = x1 + dx[direction];
	int16_t newY1 = y1 + dy[direction];


	board.moveSpace(x1, y1, newX1, newY1);

	board.moveSpace(x2, y2, x1, y1);

	return true;

}

