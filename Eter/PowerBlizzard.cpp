#include "PowerBlizzard.h"



const std::string PowerBlizzard::m_name = "PowerBlizzard";
const std::string PowerBlizzard::m_description = "Place this card onto any space and choose a row or column that it will impact. The opponent cannot play any cards onto the chosen row/column during his next turn. Condition - your opponent must have a free space to play a card. Remove this card from play after the next opponent turn. ";

PowerBlizzard::PowerBlizzard()
{
}

std::string PowerBlizzard::getName() const
{
	return m_name;
}

std::string PowerBlizzard::getDescription() const
{
	return m_description;
}

