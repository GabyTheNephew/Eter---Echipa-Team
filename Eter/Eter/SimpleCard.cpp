#include "SimpleCard.h"

SimpleCard::SimpleCard()
{
}

SimpleCard::SimpleCard(int8_t value):
	m_value{value}
{}

SimpleCard::~SimpleCard(){}

SimpleCard& SimpleCard::operator=(const SimpleCard& other)
{
	if (this != &other) {    
		m_value = other.m_value;
	}
	return *this;
}

int8_t SimpleCard::getValue()
{
	return m_value;
}

void SimpleCard::setValue(int8_t value)
{
	m_value = value;
}


