#pragma once
#include "SimpleCard.h"
#include <iostream>
#include <string_view>
#include <cstdint>

class EterCard : public SimpleCard
{
private:

	std::string m_name;

public:

	EterCard(std::string_view color);
	~EterCard();

	std::string getName();
	void setName(std::string_view name);
};

