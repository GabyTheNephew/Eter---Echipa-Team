#include "SimpleCard.h"

SimpleCard::SimpleCard():
	m_value{0},
	m_color{""}
{
}

SimpleCard::SimpleCard(int8_t value, std::string_view color):
	m_value{value},
	m_color{color}
{}

SimpleCard::~SimpleCard(){}

SimpleCard& SimpleCard::operator=(const SimpleCard& other)
{
	if (this != &other) {    
		m_value = other.m_value;
	}
	return *this;
}

int8_t SimpleCard::getValue()const
{
	return m_value;
}

std::string SimpleCard::getColor() const
{
	return m_color;
}

void SimpleCard::setValue(int8_t value)
{
	m_value = value;
}

void SimpleCard::setColor(std::string_view color)
{
	m_color = color;
}

std::ostream& operator<<(std::ostream& os, const SimpleCard& card)
{
	os << static_cast<int>(card.m_value) << card.m_color;
	return os;
}