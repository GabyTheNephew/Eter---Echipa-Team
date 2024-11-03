#include "SpecialCards.h"



SpecialCards::SpecialCards(std::string_view name, std::string_view description):
	m_name{name},
	m_description{description}
{

}

void SpecialCards::setName(std::string_view name)
{
	m_name = name;
}

void SpecialCards::setDescription(std::string_view description)
{
	m_description = description;
}

void SpecialCards::setIsPlayable(bool isPlayable)
{
	m_isPlayable = isPlayable;
}

std::string_view SpecialCards::getName() const
{
	return m_name;
}

std::string_view SpecialCards::getDescription() const
{
	return m_description;
}

bool SpecialCards::getIsPlayable() const
{
	return m_isPlayable;
}
