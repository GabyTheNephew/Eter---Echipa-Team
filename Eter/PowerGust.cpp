#include "PowerGust.h"


const std::string PowerGust::m_name = "Gust";
const std::string PowerGust::m_description = "Move any card on the playing field horizontally orvertically onto a neighboringcard of a lower value.Affects individual cards not stacks.";

PowerGust::PowerGust()
{
}

std::string_view PowerGust::getName() const
{
	return m_name;
}

std::string_view PowerGust::getDescription() const
{
	return std::string_view();
}

bool PowerGust::checkGustPower(Board& board,int16_t x,int16_t y)
{
	if (x < 0 || y < 0 || x >= board.getRowSize() || y >= board.getColumnSize()) 
		return false;

	if (board[{x, y}].empty())
		return false;


	int16_t currentValue = board[{x, y}].back().getValue();


	std::vector<std::pair<int16_t, int16_t>> neighbors = {
	   {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}
	};


	for (const auto& [nx, ny] : neighbors) {
		if (nx >= 0 && ny >= 0 && nx < board.getRowSize() && ny < board.getColumnSize())
		{
			if (!board[{nx, ny}].empty() && board[{nx, ny}].front().getValue() < currentValue)
			{
				return true;
			}
		}
	}

	return false;
}

void PowerGust::playGustPower(Board& board, int16_t sourceX, int16_t sourceY, int16_t targetX, int16_t targetY)
{
	SimpleCard cardToMove = board[{sourceX, sourceY}].back();
	board[{sourceX, sourceY}].pop_back();
	board.pushCard(cardToMove, { targetX, targetY });
}
