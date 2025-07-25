#include "PowerWhirlpool.h"

const std::string PowerWhirlpool::m_name = "WHIRLPOOL";
const std::string PowerWhirlpool::m_description = "Move onto empty space two cards from different adjacent spaces in a straight line. The card of lower value goes to the bottom of the stack.If the cards are of equal values, you choose which one will be on top. Affects individual cards not stacks.";


PowerWhirlpool::PowerWhirlpool() 
{
}

std::string PowerWhirlpool::getName() const
{
	return m_name;
}

std::string PowerWhirlpool::getDescription() const
{
	return m_description;
}

void PowerWhirlpool::playWhirlpoolPower(Board& board, int16_t row, int16_t yFirst, int16_t ySecound)
{
	if(yFirst>ySecound)
		std::swap(yFirst, ySecound);

	int16_t middle = yFirst + 1;

	SimpleCard firstCard = board[{row, yFirst}].back();
	SimpleCard secondCard = board[{row, ySecound}].back();

	board[{row, yFirst}].pop_back();
	board[{row, ySecound}].pop_back();

	if (firstCard.getValue() >= secondCard.getValue())
	{
		board.pushCard(secondCard, { row, middle });
		board.pushCard(firstCard, { row, middle });
	}
	else
	{
		board.pushCard(firstCard, { row, middle });
		board.pushCard(secondCard, { row, middle });
	}
}

bool PowerWhirlpool::checkWhirlpoolPower(Board& board, int16_t row, int16_t yFirst, int16_t ySecond)
{
		if(row<0||row >= board.getRowSize() || yFirst < 0 || yFirst >= board.getColumnSize() || ySecond < 0 || ySecond >= board.getColumnSize())
		{
			return false;
		}

		if (yFirst > ySecond) 
			std::swap(yFirst, ySecond);


		return (ySecond - yFirst == 2) && board[{row, yFirst}].size() == 1 && board[{row, ySecond}].size() == 1 && board[{row, yFirst + 1}].empty();
			
}
