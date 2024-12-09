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

bool PowerDestruction::checkDestructionPower(Player& player)
{
	if (player.getPastVector().empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void PowerDestruction::playDestructionPower(Player& player)
{

	std::string color = player.GetVectorColor();


	if (checkDestructionPower(player))
	{
		for (auto it = player.getPastVector().rbegin(); it != player.getPastVector().rend(); ++it)
		{
			auto card = *it;

			if (color == "Red")
			{
				if (card.getColor() == Color::usedBlue)
				{
					player.deleteCardFromPastVector(card);
					break;
				}
			}
			if (color == "Blue")
			{
				if (card.getColor() == Color::usedRed)
				{
					player.deleteCardFromPastVector(card);
					break;
				}
			}
		}
	}
}

