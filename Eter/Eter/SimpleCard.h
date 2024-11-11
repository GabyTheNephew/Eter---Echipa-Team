#pragma once
#include <iostream>
#include <cstdint>
#include <string_view>

class SimpleCard
{
private:
	uint8_t m_value;
	std::string m_color;
public:

	SimpleCard();
	SimpleCard(uint8_t value, std::string_view color);
	~SimpleCard();
	SimpleCard& operator=(const SimpleCard& other);

	uint8_t getValue()const;
	std::string getColor()const;

	void setValue(uint8_t value);
	void setColor(std::string_view color);

	friend std::ostream& operator<<(std::ostream& os, const SimpleCard& card);
};

