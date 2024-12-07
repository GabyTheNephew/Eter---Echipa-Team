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
	bool m_isIllusion;
public:

	SimpleCard();
	SimpleCard(uint8_t value, const Color& color);
	virtual ~SimpleCard();
	SimpleCard& operator=(const SimpleCard& other);

	uint8_t getValue()const;
	Color getColor()const;

	void setValue(uint8_t value);
	void setColor(Color color);

	void setIllusionState(bool value);
	bool getIllusionState()const;

	friend std::ostream& operator<<(std::ostream& os, const SimpleCard& card);
};

