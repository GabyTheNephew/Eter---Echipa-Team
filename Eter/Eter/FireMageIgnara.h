#pragma once
#include "Mage.h"
#include <iostream>
#include <string_view>

class FireMageIgnara : public Mage
{
public:
	FireMageIgnara(std::string_view name, std::string_view description);
	void playMage()const override;
};

