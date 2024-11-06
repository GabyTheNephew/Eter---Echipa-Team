#pragma once
#include <iostream>
#include <cstdint>

class SimpleCard
{
private:
	int8_t m_value;
public:

	SimpleCard();
	SimpleCard(int8_t value);
	~SimpleCard();
	SimpleCard& operator=(const SimpleCard& other);

	int8_t getValue()const;
	void setValue(int8_t value);

	friend std::ostream& operator<<(std::ostream& os, const SimpleCard& card);
};

