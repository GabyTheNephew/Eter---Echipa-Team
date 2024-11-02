#include "EterCard.h"

EterCard::EterCard():
	m_name{ "Eter" },
	SimpleCard(5)
{
}

EterCard::~EterCard()
{
}

std::string_view EterCard::getName()
{
	return m_name;
}

void EterCard::setName(std::string_view name)
{
	this->m_name = name;
}

