#include "EarthMageBumbleroot.h"

EarthMageBumbleroot::EarthMageBumbleroot(std::string_view name, std::string_view description) :
	m_name{ "Pyrofang" },
	m_description{ "The chosen empty space on the playing field becomes a ’hole’. No Mage can play any cards there, no Mage Powers or Power Explosions may affect that space.Mark it with this Mage Power card or the ’hole’ marker udes in Power Explosions." }
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
	if (board[{x, y}].has_value() == false)
	{
		///make hole
	}
	return false;
}


