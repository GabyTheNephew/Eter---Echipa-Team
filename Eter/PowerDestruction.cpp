#include "PowerDestruction.h"

const std::string PowerDestruction::m_name = "DESTRUCTION";
const std::string PowerDestruction::m_desctiption = "Remove from play the last card played by the opponent.";

PowerDestruction::PowerDestruction()
{
}




std::string_view PowerDestruction::getName()const
{
	return m_name;
}

std::string_view PowerDestruction::getDescription()const
{
	return m_desctiption;
}

bool PowerDestruction::checkDestructionPower(Player& opponent)
{
	return !opponent.getPastVector().empty();
}

bool PowerDestruction::playDestructionPower(Board& board, Player& opponent)
{
	if (!checkDestructionPower(opponent))
	{
		return false;
	}


	SimpleCard lastCard = opponent.getPastVector().back();
	Color opponentColor = (lastCard.getColor() == Color::usedRed) ? Color::Red : Color::Blue;

	
	for (int i = 0; i < board.getRowSize(); i++)
	{
		for (int j = 0; j < board.getColumnSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				SimpleCard& topCard = board[{i, j}].back();
				if (topCard.getValue() == lastCard.getValue() &&
					(topCard.getColor() == opponentColor ||
						topCard.getColor() == Color::increasedRed ||
						topCard.getColor() == Color::increasedBlue ||
						topCard.getColor() == Color::decreasedRed ||
						topCard.getColor() == Color::decreasedBlue))
				{
					
					board.popCard({ i, j });
					opponent.getPastVector().pop_back();
					return true;
				}
			}
		}
	}

	return false;
}