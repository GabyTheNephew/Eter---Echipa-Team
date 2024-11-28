#include "WaterMageAqualon.h"

WaterMageAqualon::WaterMageAqualon(std::string_view name, std::string_view description) :
	m_name{ "Aqualon" },
	m_description{ "Take any row or column of 3 or more cards at the edge of the playing field and move it to a different edge.This redefines the shape of the playing field. " }
{
}

std::string WaterMageAqualon::getName() const
{
	return m_name;
}

std::string WaterMageAqualon::getDescription() const
{
	return m_description;
}

bool WaterMageAqualon::playMage(Board& board, std::string_view color, bool rowOrColumn, int8_t x)
{
	/// function
	return false;
}

