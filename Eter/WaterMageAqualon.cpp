#include "WaterMageAqualon.h"

const std::string WaterMageAqualon::m_name = "Aqualon";
const std::string WaterMageAqualon::m_description = "Take any row or column of 3 or more cards at the edge of the playing field and move it to a different edge.This redefines the shape of the playing field. ";

WaterMageAqualon::WaterMageAqualon()
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

bool WaterMageAqualon::playMage(Board& board, Color color, bool rowOrColumn, int8_t x)
{
	/// function
	return false;
}

