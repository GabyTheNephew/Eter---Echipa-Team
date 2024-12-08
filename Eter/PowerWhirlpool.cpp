#include "PowerWhirlpool.h"

PowerWhirlpool::PowerWhirlpool(std::string_view name, std::string_view description) :
	m_name{ " WHIRLPOOL" },
	m_description{ "Move onto empty space two cards from different adjacent spaces in a straight line. The card of lower value goes to the bottom of the stack.If the cards are of equal values, you choose which one will be on top. Affects individual cards not stacks." }
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

void PowerWhirlpool::playPower(Board& board,uint8_t xFirst, uint8_t yFirst, uint8_t xSecond, uint8_t ySecond)
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

bool PowerWhirlpool::checkPower(Board& board, uint8_t xFirst, uint8_t yFirst, uint8_t xSecond, uint8_t ySecond)
{
	if (ySecond < yFirst)
	{
		std::swap(ySecond, yFirst);
	}

	if (xFirst != xSecond)
	{
		//message because is not the same row
		return false;
	}

	if (abs(ySecond - yFirst) != 2)
	{
		//message pt ca nu e un singur spatiu intre pozitiile selectate
		return false;
	}

	if (board[{xFirst, ySecond - 1}].back().getValue() == NULL)
	{
		//message pt ca nu e spatiul gol dintre ele
		return false;
	}

	return true;
}
