#include "EterCard.h"

EterCard::EterCard(std::string_view color):
	m_name{ "Eter" },
	SimpleCard(5,color)
{
}

EterCard::~EterCard()
{
}

std::string EterCard::getName()
{
	return m_name;
}

void EterCard::setName(std::string_view name)
{
	this->m_name = name;
}

