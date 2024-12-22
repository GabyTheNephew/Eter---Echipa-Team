#include "PowerControlledExplosion.h"



const std::string PowerControlledExplosion::m_name = "PowerControlledExplosion";
const std::string PowerControlledExplosion::m_description = "Cause a Power Explosion. This will be the only Power Explosion for that play. If you have a ‘normal’ Power Explosion before using this Elemental Power, draw a new Power Explosion to be used with this power.";

PowerControlledExplosion::PowerControlledExplosion()
{
}

std::string PowerControlledExplosion::getName() const
{
	return m_name;
}

std::string PowerControlledExplosion::getDescription() const
{
	return m_description;
}

void PowerControlledExplosion::playControlledExplosioPower(Board& board, Player player, int16_t x, int16_t y)
{
	if (player.GetVectorColor() == "Red")
	{

		SimpleCard card = player.chooseCard();
		card.setColor(Color::IlusionRed);
		player.getPastVector().push_back(card);
		board[{x, y}].push_back(card);


	}
	if (player.GetVectorColor() == "Blue")
	{
		SimpleCard card = player.chooseCard();
		card.setColor(Color::IlusionBlue);

		player.getPastVector().push_back(card);
		board[{x, y}].push_back(card);
	}


}

bool PowerControlledExplosion::checkControlledExplosioPower(Board& board, Player& player, int16_t x, int16_t y)
{
	return true;
	//check if gamemode have explosion
}
