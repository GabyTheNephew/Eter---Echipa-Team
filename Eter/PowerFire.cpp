#include "PowerFire.h"

const std::string PowerFire::m_name = "Fire";
const std::string PowerFire::m_description = " Return to their owners’ hands all visible cards of the same value(at least 2 of them) chosen by you - also yours!In case of stacks remove only the top card.";

PowerFire::PowerFire()
{

}

std::string PowerFire::getName() const
{
	return m_name;
}

std::string PowerFire::getDescription() const
{
	return m_description;
}



bool PowerFire::playFirePower(Board& board, Player& player1, Player& player2, int16_t value)
{
	if (!checkFirePower(board, value))
	{
		return false;
	}

	for (int16_t i = 0; i < board.getRowSize(); i++)
		for (int16_t j = 0; j < board.getColumnSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				SimpleCard& topCard = board[{i, j}].back();
				if (topCard.getValue() == value)
				{
					if (topCard.getColor() == Color::Red ||topCard.getColor() == Color::increasedRed ||topCard.getColor() == Color::decreasedRed)
					{
						SimpleCard returnCard(value, Color::Red);
						player1.makeCardValid(returnCard);
					}
					else if (topCard.getColor() == Color::Blue ||topCard.getColor() == Color::increasedBlue ||topCard.getColor() == Color::decreasedBlue)
					{
						SimpleCard returnCard(value, Color::Blue);
						player2.makeCardValid(returnCard);
					}

					
					board.popCard({ i, j });
				}
			}
		}

	return true;
}

bool PowerFire::checkFirePower(Board& board, int16_t value)
{
	
	int16_t count = 0;

	for (int16_t i = 0; i < board.getRowSize(); i++)
	{
		for (int16_t j = 0; j < board.getColumnSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				if (board[{i, j}].back().getValue() == value)
				{
					count++;
				}
			}
		}
	}

	return count >= 2;
}