#pragma once
#include "SpecialCards.h"
#include <iostream>
#include <string_view>

class Mage : public SpecialCards
{
public:
	Mage(std::string_view name, std::string_view description);
	virtual void playMage()const=0;
};

