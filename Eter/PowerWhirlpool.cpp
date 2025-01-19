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

void PowerWhirlpool::playWhirlpoolPower(Board& board,int16_t xFirst, int16_t yFirst, int16_t xSecond, int16_t ySecond)
{
	

	if (board[{xFirst, yFirst}].back().getValue() > board[{xSecond, ySecond}].back().getValue())
	{
		auto card1 = SimpleCard(board[{xFirst, yFirst}].back().getValue(), board[{xFirst, yFirst}].back().getColor());
		board.pushCard(card1, { xFirst, ySecond - 1 });
		auto card2 = SimpleCard(board[{xSecond, ySecond}].back().getValue(), board[{xSecond, ySecond}].back().getColor());
		board.pushCard(card1, { xFirst, ySecond - 1 });
	}
	else
	{
		if (board[{xFirst, yFirst}].back().getValue() < board[{xSecond, ySecond}].back().getValue())
		{
			auto card2 = SimpleCard(board[{xSecond, ySecond}].back().getValue(), board[{xSecond, ySecond}].back().getColor());
			board.pushCard(card2, { xFirst, ySecond - 1 });
			auto card1 = SimpleCard(board[{xFirst, yFirst}].back().getValue(), board[{xFirst, yFirst}].back().getColor());
			board.pushCard(card1, { xFirst, ySecond - 1 });
		}
		else {
			//aici trebuie sa aleaga el
		}
	}

}

bool PowerWhirlpool::checkWhirlpoolPower(Board& board, int16_t xFirst, int16_t yFirst, int16_t xSecond, int16_t ySecond)
{
	if (ySecond < yFirst)
	{
		std::swap(ySecond, yFirst);
	}

	if (xFirst != xSecond)
	{
		
		return false;
	}

	if (abs(ySecond - yFirst) != 2)
	{
		
		return false;
	}

	if (board[{xFirst, ySecond - 1}].back().getValue() == NULL)
	{
		
		return false;
	}

	return true;
}
