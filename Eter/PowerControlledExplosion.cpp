#include "PowerControlledExplosion.h"
#include "Game.h"


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

bool PowerControlledExplosion::playControlledExplosionPower(Game& game)
{
	if (!checkControlledExplosionPower(game))
	{
		return false;
	}

	game.activateExplosion();

	return true;
}

bool PowerControlledExplosion::checkControlledExplosionPower(Game& game)
{

	if (!game.areExplosionsEnabled())
	{
		return false;
	}

	Board& board = game.getBoard();
	int minSize = (game.getCurrentGameType() == GameType::Training) ? 3 : 4;

	if (board.getRowSize() < minSize || board.getColumnSize() < minSize)
	{
		return false;
	}

	return true;
}