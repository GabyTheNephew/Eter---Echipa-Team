#include "PowerDestruction.h"

PowerDestruction::PowerDestruction()
{
	m_name = "";
	m_desctiption = "";
}

PowerDestruction::PowerDestruction(std::string_view name, std::string_view description)
	:m_name{"DESTRUCTION"}, m_desctiption{"Remove from play the last card played by the opponent."}
{
}


std::string_view PowerDestruction::getName()
{
	return m_name;
}

std::string_view PowerDestruction::getDescription()
{
	return m_desctiption;
}

bool PowerDestruction::checkPower(Player& player)
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



void PowerDestruction::playPower(Player& player)
{
	std::string color =player.GetVectorColor();


	if (checkPower(player))
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
