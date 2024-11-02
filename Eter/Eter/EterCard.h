#pragma once
#include "SimpleCard.h"
#include <iostream>
#include <string_view>
#include <cstdint>

class EterCard : public SimpleCard
{
private:

	std::string_view m_name;

public:

	EterCard();
	~EterCard();

	std::string_view getName();
	void setName(std::string_view name);
};

