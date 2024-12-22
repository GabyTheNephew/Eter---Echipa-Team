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

bool PowerAvalanche::checkAvalanchePower(Board& board, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	int rowDiff = std::abs(x1 - x2);
	int colDiff = std::abs(y1 - y2);
	if ((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1))
	{
		return true;
	}
	return false;
}

void PowerAvalanche::playAvalanchePower(Board& board, Player& player, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	if (checkAvalanchePower(board,x1,y1,x2,y2))
	{
		if (x1 - x2 == 0)
		{
			if (y2 > y1)
			{
				if (board[{x1, y2 + 1}].empty())
				{
					board.moveSpace(x2, y2, x2 + 1, y2);
					board.moveSpace(x1, y1, x2, y2);
				}
			}
			else if (y2 < y1)
			{
				if (board[{x1, y1 + 1}].empty())
				{
					board.moveSpace(x1, y1, x1 + 1, y2);
					board.moveSpace(x2, y2, x1, y1);
				}
			}
			else
			{
				std::cout << "Coordonates are invalid";
			}
		}
		else if (y1 - y2 == 0)
		{
			if (x2 > x1)
			{
				if (board[{x2 + 1, y1}].empty())
				{
					board.moveSpace(x2, y2, x2, y2 + 1);
					board.moveSpace(x1, y1, x2, y2);
				}
			}
			else if (x2 < x1)
			{
				if (board[{x1 + 1, y1}].empty())
				{
					board.moveSpace(x1, y1, x1, y1 + 1);
					board.moveSpace(x2, y2, x1, y1);
				}
			}
			else
			{
				std::cout << "Coordonates are invalid";
			}
		}
		else
		{
			std::cout << "Coordonates are invalid";
		}
	}

}

