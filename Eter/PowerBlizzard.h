#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>
#include "Player.h"
#include "SimpleCard.h"

class PowerBlizzard
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerBlizzard();

	std::string getName()const;
	std::string getDescription()const;

	
};
