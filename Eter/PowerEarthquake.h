#pragma once

#include <iostream>
#include "Board.h"
#include "Player.h"
#include <string_view>
class PowerEarthquake
{
private:
	static const std::string m_name;
	static const std::string m_description;


public:
	PowerEarthquake();

	std::string getName() const;
	std::string getDescription() const;

	void playEarthquakePower(Board& board);
	bool checkEarthquakePower(Board& board);

};
