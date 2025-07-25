#include "PowerWaterfall.h"

const std::string PowerWaterfall::m_name = "Waterfall";
const std::string PowerWaterfall::m_description = "Move all cards in a chosen column into one stack at the bottom of that column, placing them on top ofeach other, regardless of their values.The card that was visible on the top of the chosen column stays at the top of thenew stack.Condition - the chosen column must containof minimum 3 cards.";




PowerWaterfall::PowerWaterfall()
{

}

std::string_view PowerWaterfall::getName() const
{
	return m_name;
}

std::string_view PowerWaterfall::getDescription() const
{
	return m_description;
}

bool PowerWaterfall::checkWaterfallPower(Board& board)
{
	for (int16_t i = 0; i < board.getRowSize(); i++)
	{
		int ocupiedPositions = 0;
		for(int16_t j = 0; j < board.getColumnSize(); j++)
		{
			if (!board[{j, i}].empty())
			{
				ocupiedPositions++;
			}
		}

		if(ocupiedPositions >= 3)
		{
			return true;
		}
	}
	return false;
}



void PowerWaterfall::playWaterfallPower(Board& board, int16_t row, bool cascadeLeft)
{
	std::deque<SimpleCard> mergedStack;

	if (cascadeLeft)
	{
		for (int16_t col = board.getColumnSize() - 1; col >= 0; col--)
		{
			for (auto& card : board[{row, col}])
			{
				mergedStack.push_back(card);
			}
			board[{row, col}].clear();
		}

		board[{row, 0}] = std::move(mergedStack);
	}
	else
	{
		for (int16_t col = 0; col < board.getColumnSize(); col++)
		{
			for (auto& card : board[{row, col}])
			{
				mergedStack.push_back(card);
			}
			board[{row, col}].clear();
		}
		board[{row, board.getColumnSize() - 1}] = std::move(mergedStack);
	}
}
