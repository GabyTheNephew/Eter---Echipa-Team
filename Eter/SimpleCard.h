#pragma once
#include <iostream>
#include <cstdint>
#include <string_view>
#include "CardColor.h"

class SimpleCard
{
private:
	uint8_t m_value;
	Color m_color;
public:

	SimpleCard();
	SimpleCard(uint8_t value, const Color& color);
	virtual ~SimpleCard();
	SimpleCard& operator=(const SimpleCard& other);

	Color getColor()const;
	void setColor(Color color);

	uint8_t getValue()const;
	void setValue(uint8_t value);

	friend std::ostream& operator<<(std::ostream& os, const SimpleCard& card);
};

