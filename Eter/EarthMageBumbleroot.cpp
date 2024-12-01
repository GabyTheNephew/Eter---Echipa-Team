#include "EarthMageBumbleroot.h"

const std::string EarthMageBumbleroot::m_name = "Bumbleroot" ;
const std::string EarthMageBumbleroot::m_description = "The chosen empty space on the playing field becomes a ’hole’. No Mage can play any cards there, no Mage Powers or Power Explosions may affect that space.Mark it with this Mage Power card or the ’hole’ marker udes in Power Explosions.";

EarthMageBumbleroot::EarthMageBumbleroot()
{
}

std::string EarthMageBumbleroot::getName() const
{
	return m_name;
}

std::string EarthMageBumbleroot::getDescription() const
{
	return m_description;
}

bool EarthMageBumbleroot::playMage(Board& board, int8_t x, int8_t y)
{
	if (board[{x,y}].empty())
	{
		///make hole
	}
	return false;
}


