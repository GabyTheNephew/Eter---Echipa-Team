#include "PowerMist.h"



const std::string PowerMist::m_name = "PowerMist";
const std::string PowerMist::m_description = " Play an Illusion again. You cannot have 2 Illusions at the same time.";

PowerMist::PowerMist()
{
}

std::string PowerMist::getName() const
{
	return m_name;
}

std::string PowerMist::getDescription() const
{
	return m_description;
}

