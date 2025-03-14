#include "SimpleCard.h"

SimpleCard::SimpleCard():
	m_value{0},
	m_color{}
{
}

SimpleCard::SimpleCard(int16_t value, const Color& color):
	m_value{value},
	m_color{color}
{}

SimpleCard::~SimpleCard(){}

SimpleCard& SimpleCard::operator=(const SimpleCard& other)
{
	if (this != &other) {    
		m_value = other.m_value;
		m_color = other.m_color;
	}
	
	return *this;
}

int16_t SimpleCard::getValue()const
{
	return static_cast<int>(m_value);
}

Color SimpleCard::getColor() const
{
	return m_color;
}

void SimpleCard::setValue(int16_t value)
{
	m_value = value;
}

void SimpleCard::setColor(Color color)
{
	m_color = color;
}

std::ostream& operator<<(std::ostream& os, const SimpleCard& card)
{
	os << static_cast<int>(card.m_value) << ColorToString(card.m_color);
	return os;
}

bool SimpleCard::operator==(const SimpleCard & rhs) {
	return this->m_value == rhs.m_value && this->m_color == rhs.m_color;
}

