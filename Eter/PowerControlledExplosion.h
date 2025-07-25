#pragma once
#include <iostream>
#include "Board.h"
#include <string_view>
#include "Player.h"
#include "SimpleCard.h"

class Game;

class PowerControlledExplosion
{
private:

	static const std::string m_name;
	static const std::string m_description;

public:
	PowerControlledExplosion();

	std::string getName()const;
	std::string getDescription()const;

	bool playControlledExplosionPower(Game& game);
	bool checkControlledExplosionPower(Game& game);

};
