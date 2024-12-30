#pragma once
#include <iostream>
#include <cstdint>
#include <string_view>
#include "CardColor.h"

class SimpleCard
{
private:
	int16_t m_value;
	Color m_color;
public:

	SimpleCard();
	SimpleCard(int16_t value, const Color& color);
	virtual ~SimpleCard();
	SimpleCard& operator=(const SimpleCard& other);

	Color getColor()const;
	void setColor(Color color);

	int16_t getValue()const;
	void setValue(int16_t value);

	friend std::ostream& operator<<(std::ostream& os, const SimpleCard& card);
	bool operator==(const SimpleCard& rhs);

};

