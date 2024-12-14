#include "PowerMirage.h"



const std::string PowerMirage::m_name = "Mirage";
const std::string PowerMirage::m_description = " Exchange your Illusion (face-down card) with a different card from your hand (face-down).";

PowerMirage::PowerMirage()
{
}

std::string PowerMirage::getName() const
{
	return m_name;
}

std::string PowerMirage::getDescription() const
{
	return m_description;
}


